#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModelToClip = mat4(1.0);
uniform mat3 uNormalToWorld = mat3(1.0);
uniform mat4 uModelToWorld = mat4(1.0);
uniform float uTime;

out vec3 vColor;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vPos;

void main() {
        vNormal = normalize(uNormalToWorld * aNormal);
        vColor = vec3(0.5)+0.5*vNormal;
        vTexCoord = aTexCoord;
        vPos = vec3(uModelToWorld* vec4(aPosition, 1.0));//vec3(uModelToClip * vec4(aPosition, 1.0));

        gl_Position = uModelToClip * vec4(aPosition, 1.0);
        if (uTime >= 20.0f && uTime < 60.0f)
            gl_Position.y -= 0.005 * gl_Position.w * gl_Position.w;
        else if (uTime >= 60.0f) {
            float a = 0.9 * gl_Position.w;
            gl_Position.x += a*sin(gl_Position.x*0.01);
            gl_Position.y += a*(1.0f-cos(gl_Position.x*0.01));
        }
}
