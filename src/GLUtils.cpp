#include "GLUtils.h"

#include <iostream>

using namespace Eigen;

//--------------------

namespace GLUtils
{
Matrix4f perspective(int width, int height, float fov, float near, float far) {
        float n = near;                                         float f = far;
        float AR = width / (float)height;
        float r = std::tan(fov / 2.0f) * n; float l = -r;
        float t = r / AR;                                       float b = -t;

        return (Matrix4f() << 2.0f*n/(r-l) , 0.0f         ,  (r+l)/(r-l) ,  0.0f           ,
                              0.0f         , 2.0f*n/(t-b) ,  (t+b)/(t-b) ,  0.0f           ,
                              0.0f         , 0.0f         , -(f+n)/(f-n) , -2.0f*f*n/(f-n) ,
                              0.0f         , 0.0f         , -1.0f        ,  0.0f           ).finished();

}

//--------------------

GL::ShaderProgram supercool_shader(void) {
        return GL::ShaderProgram(
                "#version 330\n"
                GL_SHADER_SOURCE(
                        layout(location = 0) in vec3 aPosition;

                        void main() {
                                gl_Position = vec4(aPosition, 1);
                        }
                ),
                "#version 330\n"
                GL_SHADER_SOURCE(
                        uniform ivec2 uScreenSize;
                        uniform float uTime;

                        out vec4 cColor;

                        void main() {
                                float pi = 3.14159265359;
                                vec2 axes = uScreenSize / 2;
                                vec2 nPos = gl_FragCoord.xy / uScreenSize - 0.5;
                                axes = axes + (0.3 * axes) * sin(4 * pi * nPos.yx + 2 * uTime);

                                if (any(equal(ivec2(gl_FragCoord.xy), ivec2(axes))))
                                        cColor = vec4(0, 1, 0, 1);
                                else
                                        cColor = vec4(0.1, 0.1, 0.1, 1);
                        }
                )
        );
}
} // namespace GLUtils
