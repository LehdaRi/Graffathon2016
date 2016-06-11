#ifndef GL_HEADER_GUARD_
#define GL_HEADER_GUARD_

#include <GLFW/glfw3.h>

namespace gl
{

void import();

// OpenGL wrapper functions

// GL_VERSION_1_0
void CullFace(GLenum mode);
void FrontFace(GLenum mode);
void Hint(GLenum target, GLenum mode);
void LineWidth(GLfloat width);
void PointSize(GLfloat size);
void PolygonMode(GLenum face, GLenum mode);
void Scissor(GLint x, GLint y, GLsizei width, GLsizei height);
void TexParameterf(GLenum target, GLenum pname, GLfloat param);
void TexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
void TexParameteri(GLenum target, GLenum pname, GLint param);
void TexParameteriv(GLenum target, GLenum pname, const GLint* params);
void TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
void DrawBuffer(GLenum buf);
void Clear(GLbitfield mask);
void ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ClearStencil(GLint s);
void ClearDepth(GLdouble depth);
void StencilMask(GLuint mask);
void ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void DepthMask(GLboolean flag);
void Disable(GLenum cap);
void Enable(GLenum cap);
void Finish();
void Flush();
void BlendFunc(GLenum sfactor, GLenum dfactor);
void LogicOp(GLenum opcode);
void StencilFunc(GLenum func, GLint ref, GLuint mask);
void StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
void DepthFunc(GLenum func);
void PixelStoref(GLenum pname, GLfloat param);
void PixelStorei(GLenum pname, GLint param);
void ReadBuffer(GLenum src);
void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
void GetBooleanv(GLenum pname, GLboolean* data);
void GetDoublev(GLenum pname, GLdouble* data);
GLenum GetError();
void GetFloatv(GLenum pname, GLfloat* data);
void GetIntegerv(GLenum pname, GLint* data);
const GLubyte* GetString(GLenum name);
void GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
void GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
void GetTexParameteriv(GLenum target, GLenum pname, GLint* params);
void GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat* params);
void GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params);
GLboolean IsEnabled(GLenum cap);
void DepthRange(GLdouble near, GLdouble far);
void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

// GL_VERSION_1_1
void DrawArrays(GLenum mode, GLint first, GLsizei count);
void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
void GetPointerv(GLenum pname, void** params);
void PolygonOffset(GLfloat factor, GLfloat units);
void CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
void TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void BindTexture(GLenum target, GLuint texture);
void DeleteTextures(GLsizei n, const GLuint* textures);
void GenTextures(GLsizei n, GLuint* textures);
GLboolean IsTexture(GLuint texture);

// GL_VERSION_1_2
void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
void TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
void TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
void CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

// GL_VERSION_1_3
void ActiveTexture(GLenum texture);
void SampleCoverage(GLfloat value, GLboolean invert);
void CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
void CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
void CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
void CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
void CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
void GetCompressedTexImage(GLenum target, GLint level, void* img);

// GL_VERSION_1_4
void BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void MultiDrawArrays(GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount);
void MultiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount);
void PointParameterf(GLenum pname, GLfloat param);
void PointParameterfv(GLenum pname, const GLfloat* params);
void PointParameteri(GLenum pname, GLint param);
void PointParameteriv(GLenum pname, const GLint* params);
void BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void BlendEquation(GLenum mode);

// GL_VERSION_1_5
void GenQueries(GLsizei n, GLuint* ids);
void DeleteQueries(GLsizei n, const GLuint* ids);
GLboolean IsQuery(GLuint id);
void BeginQuery(GLenum target, GLuint id);
void EndQuery(GLenum target);
void GetQueryiv(GLenum target, GLenum pname, GLint* params);
void GetQueryObjectiv(GLuint id, GLenum pname, GLint* params);
void GetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params);
void BindBuffer(GLenum target, GLuint buffer);
void DeleteBuffers(GLsizei n, const GLuint* buffers);
void GenBuffers(GLsizei n, GLuint* buffers);
GLboolean IsBuffer(GLuint buffer);
void BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
void GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void* data);
void* MapBuffer(GLenum target, GLenum access);
GLboolean UnmapBuffer(GLenum target);
void GetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
void GetBufferPointerv(GLenum target, GLenum pname, void** params);

// GL_VERSION_2_0
void BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
void DrawBuffers(GLsizei n, const GLenum* bufs);
void StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
void StencilMaskSeparate(GLenum face, GLuint mask);
void AttachShader(GLuint program, GLuint shader);
void BindAttribLocation(GLuint program, GLuint index, const GLchar* name);
void CompileShader(GLuint shader);
GLuint CreateProgram();
GLuint CreateShader(GLenum type);
void DeleteProgram(GLuint program);
void DeleteShader(GLuint shader);
void DetachShader(GLuint program, GLuint shader);
void DisableVertexAttribArray(GLuint index);
void EnableVertexAttribArray(GLuint index);
void GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
GLint GetAttribLocation(GLuint program, const GLchar* name);
void GetProgramiv(GLuint program, GLenum pname, GLint* params);
void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void GetShaderiv(GLuint shader, GLenum pname, GLint* params);
void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source);
GLint GetUniformLocation(GLuint program, const GLchar* name);
void GetUniformfv(GLuint program, GLint location, GLfloat* params);
void GetUniformiv(GLuint program, GLint location, GLint* params);
void GetVertexAttribdv(GLuint index, GLenum pname, GLdouble* params);
void GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
void GetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
void GetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
GLboolean IsProgram(GLuint program);
GLboolean IsShader(GLuint shader);
void LinkProgram(GLuint program);
void ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
void UseProgram(GLuint program);
void Uniform1f(GLint location, GLfloat v0);
void Uniform2f(GLint location, GLfloat v0, GLfloat v1);
void Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void Uniform1i(GLint location, GLint v0);
void Uniform2i(GLint location, GLint v0, GLint v1);
void Uniform3i(GLint location, GLint v0, GLint v1, GLint v2);
void Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void Uniform1fv(GLint location, GLsizei count, const GLfloat* value);
void Uniform2fv(GLint location, GLsizei count, const GLfloat* value);
void Uniform3fv(GLint location, GLsizei count, const GLfloat* value);
void Uniform4fv(GLint location, GLsizei count, const GLfloat* value);
void Uniform1iv(GLint location, GLsizei count, const GLint* value);
void Uniform2iv(GLint location, GLsizei count, const GLint* value);
void Uniform3iv(GLint location, GLsizei count, const GLint* value);
void Uniform4iv(GLint location, GLsizei count, const GLint* value);
void UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void ValidateProgram(GLuint program);
void VertexAttrib1d(GLuint index, GLdouble x);
void VertexAttrib1dv(GLuint index, const GLdouble* v);
void VertexAttrib1f(GLuint index, GLfloat x);
void VertexAttrib1fv(GLuint index, const GLfloat* v);
void VertexAttrib1s(GLuint index, GLshort x);
void VertexAttrib1sv(GLuint index, const GLshort* v);
void VertexAttrib2d(GLuint index, GLdouble x, GLdouble y);
void VertexAttrib2dv(GLuint index, const GLdouble* v);
void VertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
void VertexAttrib2fv(GLuint index, const GLfloat* v);
void VertexAttrib2s(GLuint index, GLshort x, GLshort y);
void VertexAttrib2sv(GLuint index, const GLshort* v);
void VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void VertexAttrib3dv(GLuint index, const GLdouble* v);
void VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void VertexAttrib3fv(GLuint index, const GLfloat* v);
void VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);
void VertexAttrib3sv(GLuint index, const GLshort* v);
void VertexAttrib4Nbv(GLuint index, const GLbyte* v);
void VertexAttrib4Niv(GLuint index, const GLint* v);
void VertexAttrib4Nsv(GLuint index, const GLshort* v);
void VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void VertexAttrib4Nubv(GLuint index, const GLubyte* v);
void VertexAttrib4Nuiv(GLuint index, const GLuint* v);
void VertexAttrib4Nusv(GLuint index, const GLushort* v);
void VertexAttrib4bv(GLuint index, const GLbyte* v);
void VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void VertexAttrib4dv(GLuint index, const GLdouble* v);
void VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void VertexAttrib4fv(GLuint index, const GLfloat* v);
void VertexAttrib4iv(GLuint index, const GLint* v);
void VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void VertexAttrib4sv(GLuint index, const GLshort* v);
void VertexAttrib4ubv(GLuint index, const GLubyte* v);
void VertexAttrib4uiv(GLuint index, const GLuint* v);
void VertexAttrib4usv(GLuint index, const GLushort* v);
void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

// GL_VERSION_2_1
void UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

// GL_VERSION_3_0
void ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void GetBooleani_v(GLenum target, GLuint index, GLboolean* data);
void GetIntegeri_v(GLenum target, GLuint index, GLint* data);
void Enablei(GLenum target, GLuint index);
void Disablei(GLenum target, GLuint index);
GLboolean IsEnabledi(GLenum target, GLuint index);
void BeginTransformFeedback(GLenum primitiveMode);
void EndTransformFeedback();
void BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void BindBufferBase(GLenum target, GLuint index, GLuint buffer);
void TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
void GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
void ClampColor(GLenum target, GLenum clamp);
void BeginConditionalRender(GLuint id, GLenum mode);
void EndConditionalRender();
void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
void GetVertexAttribIiv(GLuint index, GLenum pname, GLint* params);
void GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params);
void VertexAttribI1i(GLuint index, GLint x);
void VertexAttribI2i(GLuint index, GLint x, GLint y);
void VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z);
void VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w);
void VertexAttribI1ui(GLuint index, GLuint x);
void VertexAttribI2ui(GLuint index, GLuint x, GLuint y);
void VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z);
void VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void VertexAttribI1iv(GLuint index, const GLint* v);
void VertexAttribI2iv(GLuint index, const GLint* v);
void VertexAttribI3iv(GLuint index, const GLint* v);
void VertexAttribI4iv(GLuint index, const GLint* v);
void VertexAttribI1uiv(GLuint index, const GLuint* v);
void VertexAttribI2uiv(GLuint index, const GLuint* v);
void VertexAttribI3uiv(GLuint index, const GLuint* v);
void VertexAttribI4uiv(GLuint index, const GLuint* v);
void VertexAttribI4bv(GLuint index, const GLbyte* v);
void VertexAttribI4sv(GLuint index, const GLshort* v);
void VertexAttribI4ubv(GLuint index, const GLubyte* v);
void VertexAttribI4usv(GLuint index, const GLushort* v);
void GetUniformuiv(GLuint program, GLint location, GLuint* params);
void BindFragDataLocation(GLuint program, GLuint color, const GLchar* name);
GLint GetFragDataLocation(GLuint program, const GLchar* name);
void Uniform1ui(GLint location, GLuint v0);
void Uniform2ui(GLint location, GLuint v0, GLuint v1);
void Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
void Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void Uniform1uiv(GLint location, GLsizei count, const GLuint* value);
void Uniform2uiv(GLint location, GLsizei count, const GLuint* value);
void Uniform3uiv(GLint location, GLsizei count, const GLuint* value);
void Uniform4uiv(GLint location, GLsizei count, const GLuint* value);
void TexParameterIiv(GLenum target, GLenum pname, const GLint* params);
void TexParameterIuiv(GLenum target, GLenum pname, const GLuint* params);
void GetTexParameterIiv(GLenum target, GLenum pname, GLint* params);
void GetTexParameterIuiv(GLenum target, GLenum pname, GLuint* params);
void ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value);
void ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value);
void ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value);
void ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte* GetStringi(GLenum name, GLuint index);
GLboolean IsRenderbuffer(GLuint renderbuffer);
void BindRenderbuffer(GLenum target, GLuint renderbuffer);
void DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
void GenRenderbuffers(GLsizei n, GLuint* renderbuffers);
void RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
GLboolean IsFramebuffer(GLuint framebuffer);
void BindFramebuffer(GLenum target, GLuint framebuffer);
void DeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
void GenFramebuffers(GLsizei n, GLuint* framebuffers);
GLenum CheckFramebufferStatus(GLenum target);
void FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
void GenerateMipmap(GLenum target);
void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void* MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
void BindVertexArray(GLuint array);
void DeleteVertexArrays(GLsizei n, const GLuint* arrays);
void GenVertexArrays(GLsizei n, GLuint* arrays);
GLboolean IsVertexArray(GLuint array);

// GL_VERSION_3_1
void DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
void DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
void TexBuffer(GLenum target, GLenum internalformat, GLuint buffer);
void PrimitiveRestartIndex(GLuint index);
void CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
void GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
GLuint GetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
void GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
void GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

// GL_VERSION_3_2
void DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);
void MultiDrawElementsBaseVertex(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount, const GLint* basevertex);
void ProvokingVertex(GLenum mode);
GLsync FenceSync(GLenum condition, GLbitfield flags);
GLboolean IsSync(GLsync sync);
void DeleteSync(GLsync sync);
GLenum ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
void WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
void GetInteger64v(GLenum pname, GLint64* data);
void GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
void GetInteger64i_v(GLenum target, GLuint index, GLint64* data);
void GetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params);
void FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
void TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void GetMultisamplefv(GLenum pname, GLuint index, GLfloat* val);
void SampleMaski(GLuint maskNumber, GLbitfield mask);

// GL_VERSION_3_3
void BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar* name);
GLint GetFragDataIndex(GLuint program, const GLchar* name);
void GenSamplers(GLsizei count, GLuint* samplers);
void DeleteSamplers(GLsizei count, const GLuint* samplers);
GLboolean IsSampler(GLuint sampler);
void BindSampler(GLuint unit, GLuint sampler);
void SamplerParameteri(GLuint sampler, GLenum pname, GLint param);
void SamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param);
void SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param);
void SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param);
void SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint* param);
void SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint* param);
void GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params);
void GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint* params);
void GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params);
void GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint* params);
void QueryCounter(GLuint id, GLenum target);
void GetQueryObjecti64v(GLuint id, GLenum pname, GLint64* params);
void GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64* params);
void VertexAttribDivisor(GLuint index, GLuint divisor);
void VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);

// GL_ARB_debug_output
void DebugMessageControlARB(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
void DebugMessageInsertARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
void DebugMessageCallbackARB(GLDEBUGPROCARB callback, const void* userParam);
GLuint GetDebugMessageLogARB(GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);

} // namespace gl

#endif /* end of include guard: GL_HEADER_GUARD_ */
