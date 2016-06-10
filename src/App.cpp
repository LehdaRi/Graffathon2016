#include "App.h"

#include "GLFunctions.h"
#include "GLUtils.h"
#include "Camera.hpp"
#include <vector>
#include <string>
#include <iostream>

//--------------------

App::App(int argc, char* argv[], sf::Window& window)
:
    window_(window),
    ww_                     (window.getSize().x),
    wh_                     (window.getSize().y),
	mesh_shader_			(GL::ShaderProgram::simple()), // For rasterized rendering.
	time_					(0.0),//(glfwSetTime(0), glfwGetTime()) ),

	head_scene_				(HeadScene::simple()),
	normals_from_texture_	(true),
	cube_					(Mesh::cube()),
	torus_					(Mesh::torus(2.0f, 0.7f, 6, 6))
{
	//int width, height;
	//glfwGetFramebufferSize(window_, &width, &height);

	image_ = GL::Texture::empty_2D(ww_, wh_);
	depth_ = GL::Texture::empty_2D_depth(ww_, wh_);
	framebuffer_ = GL::FBO::simple_C0D(image_, depth_);

	glClearColor(0.15, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);
}

void App::loop(void) {
	while (window_.isOpen()) {
		//time_ = glfwGetTime();

		//int width, height;
		//glfwGetFramebufferSize(window_, &width, &height);
		time_ += 0.001f;

		glClearColor(0.35, 0.1, 0.1, 1);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, framebuffer_);
		head_scene_.render(time_, normals_from_texture_, ww_, wh_, framebuffer_);

		glClearColor(0.15, 0.1, 0.1, 1);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_on_cube(image_, ww_, wh_);

		window_.display();

		handleEvents();
		//glfwPollEvents();
	}
}

void App::raymarch(int width, int height, GLuint framebuffer) {
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	// Camera.
	auto eye				= Eigen::Vector3f(3*std::sin(0.6*time_), 2, 3*std::cos(0.6*time_));
	auto target				= Eigen::Vector3f::Zero();
	auto up					= Eigen::Vector3f(0, 1, 0);
	Eigen::Matrix3f FRU		= GLUtils::ray_look_at(eye, target, up, width, height);
	auto data_ptr			= FRU.data();

	auto light				= Eigen::Vector3f(4 * std::sin(PI / 4), 2.5, 4 * std::cos(PI / 4));

	// Get the uniform locations.
	GLuint screen_size_uniform;
	GLuint eye_uniform, forward_uniform, right_uniform, up_uniform;
	GLuint far_uniform, time_uniform;
	GLuint light_uniform;

	screen_size_uniform		= glGetUniformLocation(raymarcher_shader_, "uScreenSize");

	eye_uniform				= glGetUniformLocation(raymarcher_shader_, "uEye");
	forward_uniform			= glGetUniformLocation(raymarcher_shader_, "uForward");
	right_uniform			= glGetUniformLocation(raymarcher_shader_, "uRight");
	up_uniform				= glGetUniformLocation(raymarcher_shader_, "uUp");

	far_uniform				= glGetUniformLocation(raymarcher_shader_, "uFar");
	time_uniform			= glGetUniformLocation(raymarcher_shader_, "uTime");

	light_uniform			= glGetUniformLocation(raymarcher_shader_, "uPointLight");


	// Set the uniforms and draw.
	glUseProgram(raymarcher_shader_);

	glUniform2i(screen_size_uniform, width, height);

	glUniform3fv(eye_uniform, 1, eye.data());
	glUniform3fv(forward_uniform, 1, data_ptr);
	glUniform3fv(right_uniform, 1, data_ptr + 3);
	glUniform3fv(up_uniform, 1, data_ptr + 6);

	glUniform1f(far_uniform, 120);
	glUniform1f(time_uniform, time_);

	glUniform3fv(light_uniform, 1, light.data());

	glBindVertexArray(canvas_.vao_);
	glDrawArrays(canvas_.primitive_type_, 0, canvas_.num_vertices_);
	glBindVertexArray(0);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_texture(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	GLuint texture_flag_uniform, texture_sampler_uniform;

	texture_flag_uniform = glGetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform = glGetUniformLocation(mesh_shader_, "uTextureSampler");

	glUseProgram(mesh_shader_);

	glUniform1i(texture_sampler_uniform, 1);

	glUniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	glActiveTexture(GL_TEXTURE1);
	GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(canvas_.vao_);
	glDrawArrays(canvas_.primitive_type_, 0, canvas_.num_vertices_);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, old_tex);
	glActiveTexture(old_active);
	glUniform1i(texture_flag_uniform, GL_FALSE);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_mesh(const Mesh& mesh, int width, int height, GLuint framebuffer) {
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye = Eigen::Vector3f(2 * std::sin(time_), 2, 2*std::cos(time_));
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2);
	Eigen::Matrix4f view = camera.getOrientation();
	Eigen::Matrix4f projection = camera.getPerspective();

	// We'll assume that the mesh is already in world space.
	Eigen::Matrix4f model_to_clip = projection * view;
	Eigen::Matrix3f normal_to_world = Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;

	model_to_clip_uniform = glGetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform = glGetUniformLocation(mesh_shader_, "uNormalToWorld");

	// Set the uniforms and draw.
	glUseProgram(mesh_shader_);

	glUniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	glUniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());

	glBindVertexArray(mesh.vao_);
	glDrawArrays(mesh.primitive_type_, 0, mesh.num_vertices_);
	glBindVertexArray(0);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_on_cube(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye					= Eigen::Vector3f(2 * std::sin(time_), 2, 4 * std::cos(time_));
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2);
	Eigen::Matrix4f view				= camera.getOrientation();
	Eigen::Matrix4f projection			= camera.getPerspective();

	// The cube is already in world space.
	Eigen::Matrix4f model_to_clip		= projection * view;
	Eigen::Matrix3f normal_to_world		= Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;
	GLuint texture_flag_uniform, texture_sampler_uniform;

	model_to_clip_uniform				= glGetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform				= glGetUniformLocation(mesh_shader_, "uNormalToWorld");
	texture_flag_uniform				= glGetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform				= glGetUniformLocation(mesh_shader_, "uTextureSampler");

	// Set the uniforms and draw.
	glUseProgram(mesh_shader_);

	glUniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	glUniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());
	glUniform1i(texture_sampler_uniform, 1);

	glUniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	glActiveTexture(GL_TEXTURE1);
	GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(cube_.vao_);
	glDrawArrays(cube_.primitive_type_, 0, cube_.num_vertices_);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, old_tex);
	glActiveTexture(old_active);
	glUniform1i(texture_flag_uniform, GL_FALSE);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_on_torus(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye					= Eigen::Vector3f(2 * std::sin(time_), 2, 4 * std::cos(time_));
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2);
	Eigen::Matrix4f view				= camera.getOrientation();
	Eigen::Matrix4f projection			= camera.getPerspective();

	// The cube is already in world space.
	Eigen::Matrix4f model_to_clip		= projection * view;
	Eigen::Matrix3f normal_to_world		= Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;
	GLuint texture_flag_uniform, texture_sampler_uniform;

	model_to_clip_uniform				= glGetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform				= glGetUniformLocation(mesh_shader_, "uNormalToWorld");
	texture_flag_uniform				= glGetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform				= glGetUniformLocation(mesh_shader_, "uTextureSampler");

	// Set the uniforms and draw.
	glUseProgram(mesh_shader_);

	glUniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	glUniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());
	glUniform1i(texture_sampler_uniform, 1);

	glUniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	glActiveTexture(GL_TEXTURE1);
	GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(torus_.vao_);
	glDrawArrays(torus_.primitive_type_, 0, torus_.num_vertices_);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, old_tex);
	glActiveTexture(old_active);
	glUniform1i(texture_flag_uniform, GL_FALSE);

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::handleEvents(void) {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
        {
            // end the program
            window_.close();
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
        else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                window_.close();
            break;
            default:
            break;
            }
        }
    }
}

//--------------------
