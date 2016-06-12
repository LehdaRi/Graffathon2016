#include "App.h"

#include "GLFunctions.h"
#include "GLUtils.h"
#include "Scene.hpp"
#include "Camera.hpp"
#include "MeshComponent.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <random>


using namespace Eigen;

#include <SFML/Audio.hpp>

//--------------------

App::App(int argc, char* argv[], MainWindow& window) :
    window_					(window),
	shader_			        (GL::ShaderObject::from_file(GL_VERTEX_SHADER, "shaders/VS_Simple.glsl"),
                             GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/FS_Simple.glsl")), // For rasterized rendering.
	postprocess_shader_     (GL::ShaderObject::from_file(GL_VERTEX_SHADER, "shaders/VS_PostProcess2.glsl"),
	                         GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/FS_PostProcess2.glsl")),
	time_					( (glfwSetTime(0), glfwGetTime()) ),
	renderer_               (camera_, 1024, 768, shader_, framebuffer_)
{
	// Window.
	int width, height;
	glfwGetFramebufferSize(window_, &width, &height);

	// Textures & framebuffers.
	image_ = GL::Texture::empty_2D(width, height);
	depth_ = GL::Texture::empty_2D_depth(width, height);
	framebuffer_ = GL::FBO::simple_C0D(image_, depth_);

    camera_.lookAt({0.0f, 1.0f, 25.0f}, {0.0f, 7.0f, 0.0f});
    camera_.perspective(1024, 768, PI/2, 3.0f, 250.0f);

	// Spline.
	spline_.addControlPoint({10, 1, 0});
	spline_.addControlPoint({10, 1, 10});
	spline_.addControlPoint({0, 1, 10});
	spline_.addControlPoint({-10, 1, 10});
	spline_.addControlPoint({-10, 1, 0});
	spline_.addControlPoint({-10, 1, -10});
	spline_.addControlPoint({0, 1, -10});
	spline_.addControlPoint({0, 3, -5});
	spline_.addControlPoint({0, 3, -5});
	spline_.addControlPoint({0, 5, 0});
	spline_.addControlPoint({5, 10, 5});
	spline_.addControlPoint({10, 5, 10});

	//  buildingzzzzzz
	std::default_random_engine r;
	for (auto i=0u; i<10; ++i) {
        buildings_.emplace_back(20+r()%30, 6+r()%10, Vector3f(0.0f, 0.0f, -100.0f+20.0f*i));
	}

	// Stuff.
	gl::ClearColor(0.15, 0.1, 0.1, 1);
	gl::Enable(GL_DEPTH_TEST);

	/*
	sf::Music music;
	if (!music.openFromFile("0xDEADBEEF"))
	    return -1; // error
	music.play();
	*/
}

void App::loop(void) {
	while (!glfwWindowShouldClose(window_)) {
		time_ = glfwGetTime();

        camera_.lookAt({50.0f*sinf(time_*0.125f), 10.0f, 50.0f*cosf(time_*0.125f)}, {0.0f, 10.0f, 0.0f});

		int width, height;
		glfwGetFramebufferSize(window_, &width, &height);

		gl::ClearColor(0.15, 0.1, 0.1, 1);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, framebuffer_);

		SCENE(transVisitor_);
		SCENE(spotlightVisitor_);
		SCENE(renderer_);

		gl::ClearColor(0.15, 0.1, 0.1, 1);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		postprocess(image_, width, height);

		glfwSwapBuffers(window_);
		glfwPollEvents();

		int state = glfwGetKey(window_, GLFW_KEY_GRAVE_ACCENT);
		if (state == GLFW_PRESS) {
			shader_ =
				GL::ShaderProgram(GL::ShaderObject::from_file(GL_VERTEX_SHADER, "shaders/VS_Simple.glsl"),
				                  GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/FS_Simple.glsl"));
			postprocess_shader_ =
				GL::ShaderProgram(GL::ShaderObject::from_file(GL_VERTEX_SHADER, "shaders/VS_PostProcess2.glsl"),
				                  GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/FS_PostProcess2.glsl"));
		}

		state = glfwGetKey(window_, GLFW_KEY_ESCAPE);
		if (state == GLFW_PRESS) {
			break;
		}
	}
}

void App::postprocess(const GL::Texture& input, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	auto texture_uniform = gl::GetUniformLocation(postprocess_shader_, "uScreenTexture");
	auto depth_uniform = gl::GetUniformLocation(postprocess_shader_, "uScreenDepth");
	auto screen_size_uniform = gl::GetUniformLocation(postprocess_shader_, "uScreenSize");
	auto time_uniform = gl::GetUniformLocation(postprocess_shader_, "uTime");

	gl::UseProgram(postprocess_shader_);
	gl::Uniform1i(texture_uniform, 1);
	gl::Uniform1i(depth_uniform, 2);
	gl::Uniform2i(screen_size_uniform, width, height);
	gl::Uniform1f(time_uniform, time_);

	GLint old_active; gl::GetIntegerv(GL_ACTIVE_TEXTURE, &old_active);

	gl::ActiveTexture(GL_TEXTURE1);
	GLint old_tex1; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex1);
	gl::BindTexture(GL_TEXTURE_2D, input);

	gl::ActiveTexture(GL_TEXTURE2);
	GLint old_tex2; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex2);
	gl::BindTexture(GL_TEXTURE_2D, depth_);

	gl::BindVertexArray(canvas_.vao_);
	gl::DrawArrays(canvas_.primitive_type_, 0, canvas_.num_vertices_);
	gl::BindVertexArray(0);

    gl::ActiveTexture(GL_TEXTURE1);
	gl::BindTexture(GL_TEXTURE_2D, old_tex1);
	gl::ActiveTexture(GL_TEXTURE2);
	gl::BindTexture(GL_TEXTURE_2D, old_tex2);
	gl::ActiveTexture(old_active);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

//--------------------
