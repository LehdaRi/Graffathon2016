#version 330 core

in vec2 vTexCoord;

out vec4 fColor;

uniform float uTime;
uniform ivec2 uScreenSize;
uniform sampler2D uScreenTexture;
uniform sampler2D uScreenDepth;

const float M_PI = 3.14159265358979323;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	//vec2 offset = 0.1 * vec2(rand(gl_FragCoord.xy) - 0.5, rand(gl_FragCoord.yx) - 0.5);
    //vec2 offset = vec2(0.02 * sin(2.0 * M_PI * gl_FragCoord.y / uScreenSize.y * 10), 0.0);
	//vec2 offset = vec2(1.0 / uScreenSize.y * gl_FragCoord.y, 0.0);

    float dSamples[5];
    dSamples[0] = texture(uScreenDepth, vTexCoord).r;
    dSamples[1] = texture(uScreenDepth, vTexCoord+vec2(1.0/uScreenSize.x, 0.0)).r;
    dSamples[2] = texture(uScreenDepth, vTexCoord-vec2(1.0/uScreenSize.x, 0.0)).r;
    dSamples[3] = texture(uScreenDepth, vTexCoord+vec2(0.0, 1.0/uScreenSize.y)).r;
    dSamples[4] = texture(uScreenDepth, vTexCoord-vec2(0.0, 1.0/uScreenSize.y)).r;

    float d = (pow(dSamples[0]-dSamples[1], 2.0) + pow(dSamples[0]-dSamples[2], 2.0) +
               pow(dSamples[0]-dSamples[3], 2.0) + pow(dSamples[0]-dSamples[4], 2.0))*100.0f;

	fColor = vec4(vec3(d), 1.0f) + vec4(texture(uScreenTexture, vTexCoord).rgb, 1.0f);
}
