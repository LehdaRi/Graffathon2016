#version 330 core

in vec2 vTexCoord;

out vec4 fColor;

uniform float uTime;
uniform ivec2 uScreenSize;
uniform sampler2D uScreenTexture;

const float M_PI = 3.14159265358979323;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	//vec2 offset = 0.1 * vec2(rand(gl_FragCoord.xy) - 0.5, rand(gl_FragCoord.yx) - 0.5);
    vec2 offset = vec2(0.02 * sin(2.0 * M_PI * gl_FragCoord.y / uScreenSize.y * 10), 0.0);
	//vec2 offset = vec2(1.0 / uScreenSize.y * gl_FragCoord.y, 0.0);
	fColor = vec4(texture(uScreenTexture, vTexCoord + offset).rgb, 1.0f);
}
