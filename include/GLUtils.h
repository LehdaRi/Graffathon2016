#ifndef GLUTILS_H
#define GLUTILS_H

#include <Eigen/Geometry>
#include "GLObjects.h"
#define GL_SHADER_SOURCE(CODE) #CODE
#define PI 3.1415926536f

//--------------------

namespace GLUtils
{

    GL::ShaderProgram supercool_shader(void);

} // namespace GLUtils

#endif // GLUTILS_H
