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
        glGenBuffers(1, &buffer_);
}

Buffer::Buffer(const Buffer& other) {
        glGenBuffers(1, &buffer_);

        glBindBuffer(GL_COPY_READ_BUFFER, other.buffer_);
        glBindBuffer(GL_COPY_WRITE_BUFFER, buffer_);

        GLint size, usage;
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, &usage);

        glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, usage);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

Buffer::Buffer(Buffer&& other)
:       buffer_(other.buffer_)
{
        other.buffer_ = 0;
}

Buffer::~Buffer(void) {
        glDeleteBuffers(1, &buffer_);
}

Buffer& Buffer::operator=(const Buffer& other) {
        if (this != &other) {
                glBindBuffer(GL_COPY_READ_BUFFER, other.buffer_);
                glBindBuffer(GL_COPY_WRITE_BUFFER, buffer_);

                GLint read_size, write_size, read_usage, write_usage;
                glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &read_size);
                glGetBufferParameteriv(GL_COPY_WRITE_BUFFER, GL_BUFFER_SIZE, &write_size);
                glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, &read_usage);
                glGetBufferParameteriv(GL_COPY_WRITE_BUFFER, GL_BUFFER_USAGE, &write_usage);

                if (read_size == write_size && read_usage == write_usage) {
                        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read_size);
                }
                else {
                        glBufferData(GL_COPY_WRITE_BUFFER, read_size, nullptr, read_usage);
                        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, read_size);
                }

                glBindBuffer(GL_COPY_READ_BUFFER, 0);
                glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
        }

        return *this;
}

Buffer& Buffer::operator=(Buffer&& other) {
        if (this != &other) {
                glDeleteBuffers(1, &buffer_);
                buffer_ = other.buffer_;
                other.buffer_ = 0;
        }

        return *this;
}

// Texture
Texture::Texture(void) {
        glGenTextures(1, &texture_);
}

Texture::Texture(Texture&& other)
:       texture_        (other.texture_),
        width_          (other.width_),
        height_         (other.height_)
{
        other.texture_ = 0;
        other.width_ = 0;
        other.height_ = 0;
}

Texture::~Texture(void) {
        glDeleteTextures(1, &texture_);
}

Texture& Texture::operator=(Texture&& other) {
        if (this != &other) {
                glDeleteTextures(1, &texture_);
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

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, old_tex);

        texture.width_ = width;
        texture.height_ = height;

        return texture;
}

Texture Texture::empty_2D(int width, int height) {
        Texture texture;

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, old_tex);

        texture.width_ = width;
        texture.height_ = height;

        return texture;
}

Texture Texture::empty_2D_depth(int width, int height) {
        Texture depth;

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
        glBindTexture(GL_TEXTURE_2D, depth);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, old_tex);

        depth.width_ = width;
        depth.height_ = height;

        return depth;
}

Texture Texture::empty_cube(int resolution) {
        Texture texture;

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &old_tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        for (int i = 0; i < 6; ++i) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                        resolution, resolution, 0,
                        GL_RGBA, GL_FLOAT, 0);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, old_tex);

        texture.width_ = texture.height_ = resolution;

        return texture;
}

Texture Texture::empty_cube_depth(int resolution) {
        Texture depth;

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &old_tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depth);

        for (int i = 0; i < 6; ++i) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24,
                        resolution, resolution, 0,
                        GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, old_tex);

        depth.width_ = depth.height_ = resolution;

        return depth;
}

Texture Texture::buffer_texture(const Buffer& buffer, GLenum format) {
        Texture texture;

        GLint old_tex; glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &old_tex);
        glBindTexture(GL_TEXTURE_BUFFER, texture);
        glTexBuffer(GL_TEXTURE_BUFFER, format, buffer);
        glBindTexture(GL_TEXTURE_BUFFER, old_tex);

        texture.width_ = INT_MAX; // TODO: Fix this.
        texture.height_ = 1;

        return texture;
}

// VAO
VAO::VAO(void) {
        glGenVertexArrays(1, &vao_);
}

VAO::VAO(VAO&& other)
:       vao_(other.vao_)
{
        other.vao_ = 0;
}

VAO::~VAO(void) {
        glDeleteVertexArrays(1, &vao_);
}

VAO& VAO::operator=(VAO&& other) {
        if (this != &other) {
                glDeleteVertexArrays(1, &vao_);
                vao_ = other.vao_;
                other.vao_ = 0;
        }

        return *this;
}

// FBO
FBO::FBO(void) {
        glGenFramebuffers(1, &fbo_);
}

FBO::FBO(FBO&& other)
:       fbo_(other.fbo_)
{
        other.fbo_ = 0;
}

FBO::~FBO(void) {
        glDeleteFramebuffers(1, &fbo_);
}

FBO& FBO::operator=(FBO&& other) {
        if (this != &other) {
                glDeleteFramebuffers(1, &fbo_);
                fbo_ = other.fbo_;
                other.fbo_ = 0;
        }

        return *this;
}

FBO FBO::simple_C0(const Texture& color) {
        FBO framebuffer;

        GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glClearColor(0, 0, 0, 1);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);

        const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &draw_buffer);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
                glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
                std::cerr << "Framebuffer incomplete." << std::endl;
                throw std::runtime_error("Framebuffer incomplete.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);

        return framebuffer;
}

FBO FBO::simple_C0D(const Texture& color, const Texture& depth) {
        FBO framebuffer;

        GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth, 0);

        const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &draw_buffer);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
                glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
                std::cerr << "Framebuffer incomplete." << std::endl;
                throw std::runtime_error("Framebuffer incomplete.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);

        return framebuffer;
}

// ShaderObject
ShaderObject::ShaderObject(GLenum shader_type) {
        shader_object_ = glCreateShader(shader_type);
}

ShaderObject::ShaderObject(GLenum shader_type, const char* shader_source) {
        shader_object_ = glCreateShader(shader_type);
        glShaderSource(shader_object_, 1, &shader_source, NULL);

        glCompileShader(shader_object_);

        // Check errors.
        GLint compile_status;
        glGetShaderiv(shader_object_, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
                GLint info_log_length;
                glGetShaderiv(shader_object_, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                glGetShaderInfoLog(shader_object_, info_log_length, NULL, &info_log_vector[0]);

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

                glDeleteShader(shader_object_);
                throw std::runtime_error(type_string + "compilation failed.");
        }
}

ShaderObject::ShaderObject(ShaderObject&& other)
:       shader_object_(other.shader_object_)
{
        other.shader_object_ = 0;
}

ShaderObject::~ShaderObject(void) {
        glDeleteShader(shader_object_);
}

ShaderObject& ShaderObject::operator=(ShaderObject&& other) {
        if (this != &other) {
                glDeleteShader(shader_object_);
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
                "#version 420\n"
                GL_SHADER_SOURCE(
                        layout(location = 0) in vec4 aPosition;
                        void main() {gl_Position = aPosition;}
                )
        );
}

// ShaderProgram
ShaderProgram::ShaderProgram(void) {
        shader_program_ = glCreateProgram();
}

ShaderProgram::ShaderProgram(const ShaderObject& vertex_shader, const ShaderObject& fragment_shader) {
        shader_program_ = glCreateProgram();

        glAttachShader(shader_program_, vertex_shader);
        glAttachShader(shader_program_, fragment_shader);

        if (!link()) {
                std::cerr << "Shader program linking failed. Info log:" << std::endl << get_info_log();
                throw std::runtime_error("Shader program linking failed.");
        }
}

ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source)
:       ShaderProgram(ShaderObject(GL_VERTEX_SHADER, vertex_source),
                                   ShaderObject(GL_FRAGMENT_SHADER, fragment_source))
{}

ShaderProgram::ShaderProgram(ShaderProgram&& other)
:       shader_program_(other.shader_program_)
{
        other.shader_program_ = 0;
}

ShaderProgram::~ShaderProgram(void) {
        glDeleteProgram(shader_program_);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
        if (this != &other) {
                glDeleteProgram(shader_program_);
                shader_program_ = other.shader_program_;
                other.shader_program_ = 0;
        }

        return *this;
}

GLint ShaderProgram::link(void) {
        glLinkProgram(shader_program_);

        GLint link_status;
        glGetProgramiv(shader_program_, GL_LINK_STATUS, &link_status);

        return link_status;
}

std::string ShaderProgram::get_info_log(void) {
        GLint info_log_length;

        glGetProgramiv(shader_program_, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<GLchar> info_log_vector(info_log_length + 1);
        glGetProgramInfoLog(shader_program_, info_log_length, NULL, &info_log_vector[0]);

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
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, NULL);

        glCompileShader(vertex_shader);

        // Check errors.
        GLint compile_status;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
                GLint info_log_length;
                glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                glGetShaderInfoLog(vertex_shader, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Vertex shader compilation failed. Info log:" << std::endl << &info_log_vector[0];

                glDeleteShader(vertex_shader);
                throw std::runtime_error("Vertex shader compilation failed.");
        }

        // Create the fragment shader.
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_source, NULL);

        glCompileShader(fragment_shader);

        // Check errors.
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
                GLint info_log_length;
                glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                glGetShaderInfoLog(fragment_shader, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Fragment shader compilation failed. Info log:" << std::endl << &info_log_vector[0];

                glDeleteShader(vertex_shader);
                glDeleteShader(fragment_shader);
                throw std::runtime_error("Fragment shader compilation failed.");
        }

        // Create the shader program.
        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);

        glLinkProgram(shader_program);

        // Check errors.
        GLint link_status;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
        if (link_status == GL_FALSE) {
                GLint info_log_length;
                glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<GLchar> info_log_vector(info_log_length + 1);
                glGetProgramInfoLog(shader_program, info_log_length, NULL, &info_log_vector[0]);

                std::cerr << "Shader program linking failed. Info log:" << std::endl << &info_log_vector[0];

                glDetachShader(shader_program, vertex_shader);
                glDetachShader(shader_program, fragment_shader);
                glDeleteProgram(shader_program);
                glDeleteShader(fragment_shader);
                glDeleteShader(vertex_shader);
                throw std::runtime_error("Shader program linking failed.");
        }

        // Clean up.
        glDetachShader(shader_program, vertex_shader);
        glDetachShader(shader_program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        shader_program_ = shader_program;
}*/
