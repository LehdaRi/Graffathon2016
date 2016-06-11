#version 330

in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform bool uTextureFlag = false;
uniform sampler2D uTextureSampler;

layout(location = 0) out vec4 fColor;

void main() {
        fColor = vec4(vColor, 1.0);
}
