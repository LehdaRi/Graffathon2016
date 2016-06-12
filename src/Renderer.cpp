#include "Renderer.hpp"
#include "Camera.hpp"
#include "GLObjects.h"
#include "GLUtils.h"
#include "Node.hpp"
#include "TransformationComponent.hpp"
#include "DepthVisitor.hpp"
#include "Scene.hpp"

#include <Eigen/Dense>


using namespace Eigen;


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
    auto& tc = component.node_.ref().getComponent<TransformationComponent>();
    Matrix4f model = tc.mCumulative_;

	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

	gl::Viewport(0, 0, width_, height_);

	Matrix4f model_to_clip = camera_.getPerspective() * camera_.getOrientation() * model;
	Matrix3f normal_to_world = model.block<3, 3>(0, 0).inverse().transpose();

	// Get the depth texture GLuints.
	// They will be available in the vector depthVisitor.depthTexUints.
	DepthVisitor depthVisitor;
	SCENE(depthVisitor);

	// Get the uniform locations from OpenGL.
	GLuint model_to_clip_uniform, normal_to_world_uniform, time_uniform,
           light_position_uniform, model_to_world_uniform;

	model_to_clip_uniform = gl::GetUniformLocation(shader_, "uModelToClip");
	normal_to_world_uniform = gl::GetUniformLocation(shader_, "uNormalToWorld");
	model_to_world_uniform = gl::GetUniformLocation(shader_, "uModelToWorld");
    time_uniform = gl::GetUniformLocation(shader_, "uTime");
    light_position_uniform = gl::GetUniformLocation(shader_, "uLightPosition");


	// Set the uniforms and draw.
	gl::UseProgram(shader_);

	gl::UniformMatrix4fv(model_to_clip_uniform, 1, GL_FALSE, model_to_clip.data());
	gl::UniformMatrix3fv(normal_to_world_uniform, 1, GL_FALSE, normal_to_world.data());
	gl::UniformMatrix4fv(model_to_world_uniform, 1, GL_FALSE, model.data());
	gl::Uniform3fv(light_position_uniform, 1, lightPos_.data());
    gl::Uniform1f(time_uniform, time_);

	gl::BindVertexArray(component.mesh_->vao_);
	gl::DrawArrays(component.mesh_->primitive_type_, 0, component.mesh_->num_vertices_);
	gl::BindVertexArray(0);

	gl::UseProgram(0);

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}
