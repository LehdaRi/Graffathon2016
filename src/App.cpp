#include "App.h"

#include "GLFunctions.h"
#include "GLUtils.h"
#include "Scene.hpp"
#include "Camera.hpp"
#include "MeshComponent.hpp"
#include <vector>
#include <string>
#include <iostream>

//--------------------

App::App(int argc, char* argv[], MainWindow& window) :
    window_					(window),
	shader_			        (GL::ShaderObject::from_file(GL_VERTEX_SHADER, "shaders/VS_Simple.glsl"),
                             GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/FS_Simple.glsl")), // For rasterized rendering.
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
    camera_.perspective(1024, 768, PI/2, 0.1f, 100.0f);

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

	// Stuff.
	gl::ClearColor(0.15, 0.1, 0.1, 1);
	gl::Enable(GL_DEPTH_TEST);
}

void App::loop(void) {
	while (!glfwWindowShouldClose(window_)) {
		time_ = glfwGetTime();

        camera_.lookAt({25.0f*sinf(time_), 10.0f, 25.0f*cosf(time_)}, {0.0f, 10.0f, 0.0f});

		int width, height;
		glfwGetFramebufferSize(window_, &width, &height);

		gl::ClearColor(0.15, 0.1, 0.1, 1);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SCENE(transVisitor_);
		SCENE(renderer_);
		//render_mesh_on_spline(mesh_, 1024, 768);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
}
/*
void App::raymarch(int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

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

	screen_size_uniform		= gl::GetUniformLocation(raymarcher_shader_, "uScreenSize");

	eye_uniform				= gl::GetUniformLocation(raymarcher_shader_, "uEye");
	forward_uniform			= gl::GetUniformLocation(raymarcher_shader_, "uForward");
	right_uniform			= gl::GetUniformLocation(raymarcher_shader_, "uRight");
	up_uniform				= gl::GetUniformLocation(raymarcher_shader_, "uUp");

	far_uniform				= gl::GetUniformLocation(raymarcher_shader_, "uFar");
	time_uniform			= gl::GetUniformLocation(raymarcher_shader_, "uTime");

	light_uniform			= gl::GetUniformLocation(raymarcher_shader_, "uPointLight");


	// Set the uniforms and draw.
	gl::UseProgram(raymarcher_shader_);

	gl::Uniform2i(screen_size_uniform, width, height);

	gl::Uniform3fv(eye_uniform, 1, eye.data());
	gl::Uniform3fv(forward_uniform, 1, data_ptr);
	gl::Uniform3fv(right_uniform, 1, data_ptr + 3);
	gl::Uniform3fv(up_uniform, 1, data_ptr + 6);

	gl::Uniform1f(far_uniform, 120);
	gl::Uniform1f(time_uniform, time_);

	gl::Uniform3fv(light_uniform, 1, light.data());

	gl::BindVertexArray(canvas_.vao_);
	gl::DrawArrays(canvas_.primitive_type_, 0, canvas_.num_vertices_);
	gl::BindVertexArray(0);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_texture(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	GLuint texture_flag_uniform, texture_sampler_uniform;

	texture_flag_uniform = gl::GetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform = gl::GetUniformLocation(mesh_shader_, "uTextureSampler");

	gl::UseProgram(mesh_shader_);

	gl::Uniform1i(texture_sampler_uniform, 1);

	gl::Uniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; gl::GetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	gl::ActiveTexture(GL_TEXTURE1);
	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, texture);

	gl::BindVertexArray(canvas_.vao_);
	gl::DrawArrays(canvas_.primitive_type_, 0, canvas_.num_vertices_);
	gl::BindVertexArray(0);

	gl::BindTexture(GL_TEXTURE_2D, old_tex);
	gl::ActiveTexture(old_active);
	gl::Uniform1i(texture_flag_uniform, GL_FALSE);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_mesh(const Mesh& mesh, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye = Eigen::Vector3f(10 * std::sin(time_), 2, 10*std::cos(time_));
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2);
	Eigen::Matrix4f view = camera.getOrientation();
	Eigen::Matrix4f projection = camera.getPerspective();

	// We'll assume that the mesh is already in world space.
	Eigen::Matrix4f model_to_clip = projection * view;
	Eigen::Matrix3f normal_to_world = Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;

	model_to_clip_uniform = gl::GetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform = gl::GetUniformLocation(mesh_shader_, "uNormalToWorld");

	// Set the uniforms and draw.
	gl::UseProgram(mesh_shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());

	gl::BindVertexArray(mesh.vao_);
	gl::DrawArrays(mesh.primitive_type_, 0, mesh.num_vertices_);
	gl::BindVertexArray(0);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_mesh_on_spline(const Mesh& mesh, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye = spline_.evaluate(time_);
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2);
	Eigen::Matrix4f view = camera.getOrientation();
	Eigen::Matrix4f projection = camera.getPerspective();

	// We'll assume that the mesh is already in world space.
	Eigen::Matrix4f model_to_clip = projection * view;
	Eigen::Matrix3f normal_to_world = Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;

	model_to_clip_uniform = gl::GetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform = gl::GetUniformLocation(mesh_shader_, "uNormalToWorld");

	// Set the uniforms and draw.
	gl::UseProgram(mesh_shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());

	gl::BindVertexArray(mesh.vao_);
	gl::DrawArrays(mesh.primitive_type_, 0, mesh.num_vertices_);
	gl::BindVertexArray(0);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_on_cube(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye = spline_.evaluate(time_);
	// Eigen::Vector3f eye					= Eigen::Vector3f(2 * std::sin(time_), 2, 4 * std::cos(time_));
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

	model_to_clip_uniform				= gl::GetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform				= gl::GetUniformLocation(mesh_shader_, "uNormalToWorld");
	texture_flag_uniform				= gl::GetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform				= gl::GetUniformLocation(mesh_shader_, "uTextureSampler");

	// Set the uniforms and draw.
	gl::UseProgram(mesh_shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());
	gl::Uniform1i(texture_sampler_uniform, 1);

	gl::Uniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; gl::GetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	gl::ActiveTexture(GL_TEXTURE1);
	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, texture);

	gl::BindVertexArray(cube_.vao_);
	gl::DrawArrays(cube_.primitive_type_, 0, cube_.num_vertices_);
	gl::BindVertexArray(0);

	gl::BindTexture(GL_TEXTURE_2D, old_tex);
	gl::ActiveTexture(old_active);
	gl::Uniform1i(texture_flag_uniform, GL_FALSE);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void App::render_on_torus(const GL::Texture& texture, int width, int height, GLuint framebuffer) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Viewport(0, 0, width, height);

	// Camera.
	Camera camera;
	Eigen::Vector3f eye					= Eigen::Vector3f(1 * std::sin(time_), 2, 2 * std::cos(time_));
	camera.lookAt(eye);
	camera.perspective(width, height, PI/2, 0.1f, 100.0f);
	Eigen::Matrix4f view				= camera.getOrientation();
	Eigen::Matrix4f projection			= camera.getPerspective();

	// The cube is already in world space.
	Eigen::Matrix4f model_to_clip		= projection * view;
	Eigen::Matrix3f normal_to_world		= Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;
	GLuint texture_flag_uniform, texture_sampler_uniform;

	model_to_clip_uniform				= gl::GetUniformLocation(mesh_shader_, "uModelToClip");
	normal_to_world_uniform				= gl::GetUniformLocation(mesh_shader_, "uNormalToWorld");
	texture_flag_uniform				= gl::GetUniformLocation(mesh_shader_, "uTextureFlag");
	texture_sampler_uniform				= gl::GetUniformLocation(mesh_shader_, "uTextureSampler");

	// Set the uniforms and draw.
	gl::UseProgram(mesh_shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());
	gl::Uniform1i(texture_sampler_uniform, 1);

	gl::Uniform1i(texture_flag_uniform, GL_TRUE);
	GLint old_active; gl::GetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
	gl::ActiveTexture(GL_TEXTURE1);
	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, texture);

	gl::BindVertexArray(torus_.vao_);
	gl::DrawArrays(torus_.primitive_type_, 0, torus_.num_vertices_);
	gl::BindVertexArray(0);

	gl::BindTexture(GL_TEXTURE_2D, old_tex);
	gl::ActiveTexture(old_active);
	gl::Uniform1i(texture_flag_uniform, GL_FALSE);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}
*/
//--------------------
