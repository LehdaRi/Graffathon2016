#include "HeadScene.h"

//--------------------

HeadScene HeadScene::simple(void) {
	HeadScene scene;

	// Head mesh, textures & shader.
	scene.head_ = Mesh::from_obj("res/head.obj");
	scene.head_diffuse_ = GL::Texture::from_png("res/lambertian-lores.png");
	scene.head_normal_ = GL::Texture::from_png("res/normal-lores.png");
	scene.head_shader_ = GL::ShaderProgram::from_files("shaders/head_vert.glsl", "shaders/head_frag.glsl");

	// Uniform locations.
	scene.head_uniforms_.model_to_clip			= gl::GetUniformLocation(scene.head_shader_, "uModelToClip");
	scene.head_uniforms_.model_to_view			= gl::GetUniformLocation(scene.head_shader_, "uModelToView");
	scene.head_uniforms_.normal_model_to_view	= gl::GetUniformLocation(scene.head_shader_, "uNormalModelToView");
	scene.head_uniforms_.view_to_world			= gl::GetUniformLocation(scene.head_shader_, "uViewToWorld");

	scene.head_uniforms_.diffuse_sampler		= gl::GetUniformLocation(scene.head_shader_, "uDiffuseSampler");
	scene.head_uniforms_.normal_sampler			= gl::GetUniformLocation(scene.head_shader_, "uNormalSampler");
	scene.head_uniforms_.env_sampler			= gl::GetUniformLocation(scene.head_shader_, "uEnvironmentSampler");

	scene.head_uniforms_.specular_color			= gl::GetUniformLocation(scene.head_shader_, "uSpecularColor");
	scene.head_uniforms_.specular_exponent		= gl::GetUniformLocation(scene.head_shader_, "uSpecularExponent");
	scene.head_uniforms_.normals_from_texture	= gl::GetUniformLocation(scene.head_shader_, "uNormalsFromTexture");
	scene.head_uniforms_.use_env				= gl::GetUniformLocation(scene.head_shader_, "uUseEnv");

	scene.head_uniforms_.dir_light_sampler		= gl::GetUniformLocation(scene.head_shader_, "uDirLightSampler");
	scene.head_uniforms_.point_light_sampler	= gl::GetUniformLocation(scene.head_shader_, "uPointLightSampler");

	// Static uniforms.
	gl::UseProgram(scene.head_shader_);
	gl::Uniform1i(scene.head_uniforms_.diffuse_sampler, 0);
	gl::Uniform1i(scene.head_uniforms_.normal_sampler, 1);
	gl::Uniform1i(scene.head_uniforms_.env_sampler, 2);

	gl::Uniform3f(scene.head_uniforms_.specular_color, 0.35f, 0.35f, 0.35f);
	gl::Uniform1f(scene.head_uniforms_.specular_exponent, 40.0f);

	gl::Uniform1i(scene.head_uniforms_.dir_light_sampler, 3);
	gl::Uniform1i(scene.head_uniforms_.point_light_sampler, 4);
	gl::UseProgram(0);

	// Static texture units.
	gl::ActiveTexture(GL_TEXTURE0);
	gl::BindTexture(GL_TEXTURE_2D, scene.head_diffuse_);
	gl::ActiveTexture(GL_TEXTURE1);
	gl::BindTexture(GL_TEXTURE_2D, scene.head_normal_);

	return scene;
}
