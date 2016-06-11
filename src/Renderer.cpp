#include "Renderer.hpp"
#include "Camera.hpp"
#include "GLObjects.h"
#include "GLUtils.h"

#include <Eigen/Dense>


Renderer::Renderer(Camera& camera, uint32_t width, uint32_t height,
                   GL::ShaderProgram& shader, GL::FBO& framebuffer) :
    camera_         (camera),
    width_          (width),
    height_         (height),
    shader_         (shader),
    framebuffer_    (framebuffer)
{
	camera_.perspective(width, height, PI/2);
}

void Renderer::operator()(MeshComponent& component) {
	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, 0);//framebuffer_);

	gl::Viewport(0, 0, width_, height_);

	// Camera.
	float time_ = 0.0f; //TEMP
	Eigen::Vector3f eye = Eigen::Vector3f(10 * std::sin(time_), 2, 10*std::cos(time_));
	camera_.lookAt(eye);
	//Eigen::Matrix4f view = camera.getOrientation();
	//Eigen::Matrix4f projection = camera.getPerspective();

	// We'll assume that the mesh is already in world space.
	Eigen::Matrix4f model_to_clip = camera_.getPerspective() * camera_.getOrientation();
	Eigen::Matrix3f normal_to_world = Eigen::Matrix3f::Identity();

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform;

	model_to_clip_uniform = gl::GetUniformLocation(shader_, "uModelToClip");
	normal_to_world_uniform = gl::GetUniformLocation(shader_, "uNormalToWorld");

	// Set the uniforms and draw.
	gl::UseProgram(shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());

	gl::BindVertexArray(component.mesh_->vao_);
	gl::DrawArrays(component.mesh_->primitive_type_, 0, component.mesh_->num_vertices_);
	gl::BindVertexArray(0);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}
