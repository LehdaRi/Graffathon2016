#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include "gl_api.hpp"
#include "GLObjects.h"

//--------------------

namespace GL
{
inline void clear(GLbitfield mask, GLuint framebuffer = 0) {
        GLint old_fbo; gl::GetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
        gl::BindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        gl::Clear(mask);
        gl::BindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

void tex_to_png(const Texture& texture, const char* filename);
} // namespace GL

#endif // GLFUNCTIONS_H
