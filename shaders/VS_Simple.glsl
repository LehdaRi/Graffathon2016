#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModelToClip = mat4(1.0);
uniform mat3 uNormalToWorld = mat3(1.0);

out vec3 vColor;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
        vNormal = normalize(uNormalToWorld * aNormal);
        vColor = vec3(0.5)+0.5*vNormal;
        vTexCoord = aTexCoord;

        gl_Position = uModelToClip * vec4(aPosition, 1.0);
}
