#include "GLObjects.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <lodepng.h>
#include <climits>

//--------------------

namespace GL
{
// Buffer
Buffer::Buffer(void) {
	gl::GenBuffers(1, &buffer_);
}

Buffer::Buffer(const Buffer& other) {
	gl::GenBuffers(1, &buffer_);

	gl::BindBuffer(GL_COPY_READ_BUFFER, other.buffer_);
	gl::BindBuffer(GL_COPY_WRITE_BUFFER, buffer_);

	GLint size, usage;
	gl::GetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	gl::GetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, &usage);

	gl::BufferData(GL_COPY_WRITE_BUFFER, size, nullptr, usage);
	gl::CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

	gl::BindBuffer(GL_COPY_READ_BUFFER, 0);
	gl::BindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

Buffer::Buffer(Buffer&& other) :
	buffer_(other.buffer_)
{
	other.buffer_ = 0;
}

Buffer::~Buffer(void) {
	gl::DeleteBuffers(1, &buffer_);
}

Buffer& Buffer::operator=(const Buffer& other) {
	if (this != &other) {
		gl::BindBuffer(GL_COPY_READ_BUFFER, other.buffer_);
		gl::BindBuffer(GL_COPY_WRITE_BUFFER, buffer_);

		GLint read_size, write_size, read_usage, write_usage;
		gl::GetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &read_size);
		gl::GetBufferParameteriv(GL_COPY_WRITE_BUFFER, GL_BUFFER_SIZE, &write_size);
		gl::GetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, &read_usage);
		gl::GetBufferParameteriv(GL_COPY_WRITE_BUFFER, GL_BUFFER_USAGE, &write_usage);

		if (read_size == write_size && read_usage == write_usage) {
			gl::CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read_size);
		}
		else {
			gl::BufferData(GL_COPY_WRITE_BUFFER, read_size, nullptr, read_usage);
			gl::CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read_size);
		}

		gl::BindBuffer(GL_COPY_READ_BUFFER, 0);
		gl::BindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}

	return *this;
}

Buffer& Buffer::operator=(Buffer&& other) {
	if (this != &other) {
		gl::DeleteBuffers(1, &buffer_);
		buffer_ = other.buffer_;
		other.buffer_ = 0;
	}

	return *this;
}

// Texture
Texture::Texture(void) {
	gl::GenTextures(1, &texture_);
}

Texture::Texture(Texture&& other) :
	texture_ (other.texture_),
	width_   (other.width_),
	height_  (other.height_)
{
	other.texture_ = 0;
	other.width_ = 0;
	other.height_ = 0;
}

Texture::~Texture(void) {
	gl::DeleteTextures(1, &texture_);
}

Texture& Texture::operator=(Texture&& other) {
	if (this != &other) {
		gl::DeleteTextures(1, &texture_);
		texture_ = other.texture_;
		other.texture_ = 0;

		width_ = other.width_;
		height_ = other.height_;
		other.width_ = 0;
		other.height_ = 0;
	}

	return *this;
}

Texture Texture::from_png(const char* filename) { // TODO: Error handling & filtering.
	assert(filename != nullptr);

	std::vector<unsigned char> ud_image;
	unsigned int width, height;
	lodepng::decode(ud_image, width, height, filename);

	std::vector<unsigned char> image(ud_image.size());
	for (size_t i = 0; i < height; ++i) {
		for (size_t j = 0; j < 4 * width; ++j)
			image[4 * width * i + j] = ud_image[4 * width * (height - (1 + i)) + j];
	}

	Texture texture;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, texture);
	gl::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gl::GenerateMipmap(GL_TEXTURE_2D);
	gl::BindTexture(GL_TEXTURE_2D, old_tex);

	texture.width_ = width;
	texture.height_ = height;

	return texture;
}

Texture Texture::empty_2D(int width, int height) {
	Texture texture;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, texture);
	gl::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl::BindTexture(GL_TEXTURE_2D, old_tex);

	texture.width_ = width;
	texture.height_ = height;

	return texture;
}

Texture Texture::empty_2D_multisample(int width, int height, int num_samples)
{
	Texture texture;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	gl::TexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA, width, height, false);
	gl::TexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl::BindTexture(GL_TEXTURE_2D_MULTISAMPLE, old_tex);

	texture.width_ = width;
	texture.height_ = height;

	return texture;
}

Texture Texture::empty_2D_multisample_depth(int width, int height, int num_samples)
{
	Texture depth;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D_MULTISAMPLE, depth);
	gl::TexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_DEPTH_COMPONENT, width, height, false);
	gl::TexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl::TexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl::BindTexture(GL_TEXTURE_2D_MULTISAMPLE, old_tex);

	depth.width_ = width;
	depth.height_ = height;

	return depth;
}

Texture Texture::empty_2D_depth(int width, int height) {
	Texture depth;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
	gl::BindTexture(GL_TEXTURE_2D, depth);

	gl::TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gl::BindTexture(GL_TEXTURE_2D, old_tex);

	depth.width_ = width;
	depth.height_ = height;

	return depth;
}

Texture Texture::empty_cube(int resolution) {
	Texture texture;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &old_tex);
	gl::BindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < 6; ++i) {
		gl::TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
		             resolution, resolution, 0,
		             GL_RGBA, GL_FLOAT, 0);
	}
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	gl::BindTexture(GL_TEXTURE_CUBE_MAP, old_tex);

	texture.width_ = texture.height_ = resolution;

	return texture;
}

Texture Texture::empty_cube_depth(int resolution) {
	Texture depth;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &old_tex);
	gl::BindTexture(GL_TEXTURE_CUBE_MAP, depth);

	for (int i = 0; i < 6; ++i) {
		gl::TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24,
		             resolution, resolution, 0,
		             GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gl::TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	gl::BindTexture(GL_TEXTURE_CUBE_MAP, old_tex);

	depth.width_ = depth.height_ = resolution;

	return depth;
}

Texture Texture::buffer_texture(const Buffer& buffer, GLenum format) {
	Texture texture;

	GLint old_tex; gl::GetIntegerv(GL_TEXTURE_BINDING_BUFFER, &old_tex);
	gl::BindTexture(GL_TEXTURE_BUFFER, texture);
	gl::TexBuffer(GL_TEXTURE_BUFFER, format, buffer);
	gl::BindTexture(GL_TEXTURE_BUFFER, old_tex);

	texture.width_ = INT_MAX; // TODO: Fix this.
	texture.height_ = 1;

	return texture;
}

// VAO
VAO::VAO(void) {
	gl::GenVertexArrays(1, &vao_);
}

VAO::VAO(VAO&& other) :
	vao_(other.vao_)
{
	other.vao_ = 0;
}

VAO::~VAO(void) {
	gl::DeleteVertexArrays(1, &vao_);
}

VAO& VAO::operator=(VAO&& other) {
	if (this != &other) {
		gl::DeleteVertexArrays(1, &vao_);
		vao_ = other.vao_;
		other.vao_ = 0;
	}

	return *this;
}

// FBO
FBO::FBO(void) {
	gl::GenFramebuffers(1, &fbo_);
}

FBO::FBO(FBO&& other) :
	fbo_(other.fbo_)
{
	other.fbo_ = 0;
}

FBO::~FBO(void) {
	gl::DeleteFramebuffers(1, &fbo_);
}

FBO& FBO::operator=(FBO&& other) {
	if (this != &other) {
		gl::DeleteFramebuffers(1, &fbo_);
		fbo_ = other.fbo_;
		other.fbo_ = 0;
	}

	return *this;
}

FBO FBO::simple_C0(const Texture& color) {
	FBO framebuffer;

	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::ClearColor(0, 0, 0, 1);

	gl::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);

	const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
	gl::DrawBuffers(1, &draw_buffer);

	GLenum status = gl::CheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		std::cerr << "Framebuffer incomplete." << std::endl;
		throw std::runtime_error("Framebuffer incomplete.");
	}

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);

	return framebuffer;
}

FBO FBO::simple_C0D(const Texture& color, const Texture& depth) {
	FBO framebuffer;

	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Enable(GL_DEPTH_TEST);
	gl::ClearColor(0, 0, 0, 1);

	gl::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);
	gl::FramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth, 0);

	const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
	gl::DrawBuffers(1, &draw_buffer);

	GLenum status = gl::CheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		std::cerr << "Framebuffer incomplete." << std::endl;
		throw std::runtime_error("Framebuffer incomplete.");
	}

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);

	return framebuffer;
}

FBO FBO::multisample_C0(const Texture& color)
{
	FBO framebuffer;

	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::ClearColor(0, 0, 0, 1);

	gl::FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color, 0);

	// const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
	// gl::DrawBuffers(1, &draw_buffer);

	GLenum status = gl::CheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		std::cerr << "Framebuffer incomplete." << std::endl;
		throw std::runtime_error("Framebuffer incomplete.");
	}

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);

	return framebuffer;
}

FBO FBO::multisample_C0D(const Texture& color, const Texture& depth)
{
	FBO framebuffer;

	GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	gl::Enable(GL_DEPTH_TEST);
	gl::ClearColor(0, 0, 0, 1);

	gl::FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color, 0);
	gl::FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth, 0);

	const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
	gl::DrawBuffers(1, &draw_buffer);

	GLenum status = gl::CheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		std::cerr << "Framebuffer incomplete." << std::endl;
		throw std::runtime_error("Framebuffer incomplete.");
	}

	gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);

	return framebuffer;
}

// ShaderObject
ShaderObject::ShaderObject(GLenum shader_type) {
	shader_object_ = gl::CreateShader(shader_type);
}

ShaderObject::ShaderObject(GLenum shader_type, const char* shader_source) {
	shader_object_ = gl::CreateShader(shader_type);
	gl::ShaderSource(shader_object_, 1, &shader_source, NULL);

	gl::CompileShader(shader_object_);

	// Check errors.
	GLint compile_status;
	gl::GetShaderiv(shader_object_, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLint info_log_length;
		gl::GetShaderiv(shader_object_, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<GLchar> info_log_vector(info_log_length + 1);
		gl::GetShaderInfoLog(shader_object_, info_log_length, NULL, &info_log_vector[0]);

		std::string type_string;
		switch (shader_type) {
		case GL_COMPUTE_SHADER:
			type_string = "Compute shader "; break;
		case GL_VERTEX_SHADER:
			type_string = "Vertex shader "; break;
		case GL_TESS_CONTROL_SHADER:
			type_string = "Tessellation control shader "; break;
		case GL_TESS_EVALUATION_SHADER:
			type_string = "Tessellation evaluation shader "; break;
		case GL_GEOMETRY_SHADER:
			type_string = "Geometry shader "; break;
		case GL_FRAGMENT_SHADER:
			type_string = "Fragment shader "; break;
		default:
			type_string = "Unknown shader ";
		}

		std::cerr << type_string << "compilation failed. Info log:" << std::endl << &info_log_vector[0];

		gl::DeleteShader(shader_object_);
		throw std::runtime_error(type_string + "compilation failed.");
	}
}

ShaderObject::ShaderObject(ShaderObject&& other) :
	shader_object_(other.shader_object_)
{
	other.shader_object_ = 0;
}

ShaderObject::~ShaderObject(void) {
	gl::DeleteShader(shader_object_);
}

ShaderObject& ShaderObject::operator=(ShaderObject&& other) {
	if (this != &other) {
		gl::DeleteShader(shader_object_);
		shader_object_ = other.shader_object_;
		other.shader_object_ = 0;
	}

	return *this;
}

ShaderObject ShaderObject::from_file(GLenum shader_type, const char* source_file) {
	assert(source_file != nullptr);

	std::ifstream source_stream(source_file);
	std::stringstream source_buffer;
	source_buffer << source_stream.rdbuf();

	return ShaderObject(shader_type, source_buffer.str().c_str());
}

ShaderObject ShaderObject::vertex_passthrough(void) {
	return ShaderObject(GL_VERTEX_SHADER,
		"#version 330 core\n"
		GL_SHADER_SOURCE(
			layout(location = 0) in vec4 aPosition;
			void main() {gl_Position = aPosition;}
		)
	);
}

// ShaderProgram
ShaderProgram::ShaderProgram(void) {
	shader_program_ = gl::CreateProgram();
}

ShaderProgram::ShaderProgram(const ShaderObject& vertex_shader, const ShaderObject& fragment_shader) {
	shader_program_ = gl::CreateProgram();

	gl::AttachShader(shader_program_, vertex_shader);
	gl::AttachShader(shader_program_, fragment_shader);

	if (!link()) {
		std::cerr << "Shader program linking failed. Info log:" << std::endl << get_info_log();
		throw std::runtime_error("Shader program linking failed.");
	}
}

ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source) :
	ShaderProgram(ShaderObject(GL_VERTEX_SHADER, vertex_source),
	                           ShaderObject(GL_FRAGMENT_SHADER, fragment_source))
{}

ShaderProgram::ShaderProgram(ShaderProgram&& other) :
	shader_program_(other.shader_program_)
{
	other.shader_program_ = 0;
}

ShaderProgram::~ShaderProgram(void) {
	gl::DeleteProgram(shader_program_);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
	if (this != &other) {
		gl::DeleteProgram(shader_program_);
		shader_program_ = other.shader_program_;
		other.shader_program_ = 0;
	}

	return *this;
}

GLint ShaderProgram::link(void) {
	gl::LinkProgram(shader_program_);

	GLint link_status;
	gl::GetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);

	return link_status;
}

std::string ShaderProgram::get_info_log(void) {
	GLint info_log_length;

	gl::GetProgramiv(shader_program_, GL_INFO_LOG_LENGTH, &info_log_length);
	std::vector<GLchar> info_log_vector(info_log_length + 1);
	gl::GetProgramInfoLog(shader_program_, info_log_length, NULL, &info_log_vector[0]);

	return &info_log_vector[0];
}

ShaderProgram ShaderProgram::from_files(const char* vertex_file, const char* fragment_file) {
	ShaderObject vertex_shader = ShaderObject::from_file(GL_VERTEX_SHADER, vertex_file);
	ShaderObject fragment_shader = ShaderObject::from_file(GL_FRAGMENT_SHADER, fragment_file);

	return ShaderProgram(vertex_shader, fragment_shader);
}
} // namespace GL

/*ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source) {
        // Create the vertex shader.
        GLuint vertex_shader = gl::CreateShader(GL_VERTEX_SHADER);
        gl::ShaderSource(vertex_shader, 1, &vertex_source, NULL);

        gl::CompileShader(vertex_shader);

        // Check errors.
        GLint compile_status;
        gl::GetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
                GLint info_log_length;
                gl::GetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                gl::GetShaderInfoLog(vertex_shader, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Vertex shader compilation failed. Info log:" << std::endl << &info_log_vector[0];

                gl::DeleteShader(vertex_shader);
                throw std::runtime_error("Vertex shader compilation failed.");
        }

        // Create the fragment shader.
        GLuint fragment_shader = gl::CreateShader(GL_FRAGMENT_SHADER);
        gl::ShaderSource(fragment_shader, 1, &fragment_source, NULL);

        gl::CompileShader(fragment_shader);

        // Check errors.
        gl::GetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
                GLint info_log_length;
                gl::GetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                gl::GetShaderInfoLog(fragment_shader, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Fragment shader compilation failed. Info log:" << std::endl << &info_log_vector[0];

                gl::DeleteShader(vertex_shader);
                gl::DeleteShader(fragment_shader);
                throw std::runtime_error("Fragment shader compilation failed.");
        }

        // Create the shader program.
        GLuint shader_program = gl::CreateProgram();
        gl::AttachShader(shader_program, vertex_shader);
        gl::AttachShader(shader_program, fragment_shader);

        gl::LinkProgram(shader_program);

        // Check errors.
        GLint link_status;
        gl::GetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
        if (link_status == GL_FALSE) {
                GLint info_log_length;
                gl::GetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                gl::GetProgramInfoLog(shader_program, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Shader program linking failed. Info log:" << std::endl << &info_log_vector[0];

                gl::DetachShader(shader_program, vertex_shader);
                gl::DetachShader(shader_program, fragment_shader);
                gl::DeleteProgram(shader_program);
                gl::DeleteShader(fragment_shader);
                gl::DeleteShader(vertex_shader);
                throw std::runtime_error("Shader program linking failed.");
        }

        // Clean up.
        gl::DetachShader(shader_program, vertex_shader);
        gl::DetachShader(shader_program, fragment_shader);
        gl::DeleteShader(vertex_shader);
        gl::DeleteShader(fragment_shader);

        shader_program_ = shader_program;
}*/
