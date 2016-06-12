#version 330

in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;

uniform bool uTextureFlag = false;
uniform sampler2D uTextureSampler;
uniform vec3 uLightPosition;

layout(location = 0) out vec4 fColor;

void main() {
    float intensity = 100.0f/dot(uLightPosition-vPos, uLightPosition-vPos);
    float diffuse = dot(vNormal, normalize(uLightPosition-vPos));
    fColor = vec4(vec3(intensity*diffuse), 1.0);
    //fColor = vec4(vec3(dot(vNormal, normalize(uLightPosition-vPos))), 1.0);
    //fColor = vec4(vColor, 1.0);
}
