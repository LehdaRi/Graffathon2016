#version 420 core

in vec2 vTexCoord;

out vec4 fColor;

uniform float uTime;
uniform ivec2 uScreenSize;
uniform sampler2DMS uScreenTexture;
uniform sampler2DMS uScreenDepth;

const float M_PI = 3.14159265358979323;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 sample16(sampler2DMS sampler, ivec2 pos) {
    vec4 s;
    for (int i=0; i<4; ++i) {
        s+=texelFetch(sampler, pos, i)*0.25;
    }
    return vec4(s.rgb, 1.0);
}


void main() {
    float lines = 300.0;
    vec2 v = vec2(0.5)-vTexCoord;
    float c = length(vec2(0.5) - vTexCoord);

    float d = c * c;
    float n = pow(rand(v + sin(uTime*10.0)), 2) * d * 0.4;

    vec2 vTexCoord2 = vTexCoord * 0.9+0.05 - v*c*0.2;

    vec3 s = vec3(0.35 + 0.4*sin(vTexCoord2.y*3.14*lines),
                  0.85 + 0.5*sin(vTexCoord2.y*3.14*lines + 3.14 * 0.66),
                  0.45 + 0.4*sin(vTexCoord2.y*3.14*lines + 3.14 * 1.33))+0.5-d;

    vec4 t = sample16(uScreenTexture, ivec2(vTexCoord2*uScreenSize)) * vec4(s, 1.0);

    vec4 color = t - t*2*vec4(n, n, n, 1.0) - d*0.15;
    float avg = (color.r + color.g + color.b) / 3;
    vec4 green = vec4(mix(vec3(0.05, 0.05, 0.05), vec3(0.15, 0.75, 0.15), avg), 1.0);

    //fColor = green;

	ivec2 texel = ivec2(vTexCoord2*uScreenSize);

    float dSamples[5];
    dSamples[0] = sample16(uScreenDepth, ivec2(vTexCoord2*uScreenSize)).r;
    dSamples[1] = sample16(uScreenDepth, ivec2((vTexCoord2+vec2(1.0/uScreenSize.x, 0.0))*uScreenSize)).r;
    dSamples[2] = sample16(uScreenDepth, ivec2((vTexCoord2-vec2(1.0/uScreenSize.x, 0.0))*uScreenSize)).r;
    dSamples[3] = sample16(uScreenDepth, ivec2((vTexCoord2+vec2(0.0, 1.0/uScreenSize.y))*uScreenSize)).r;
    dSamples[4] = sample16(uScreenDepth, ivec2((vTexCoord2-vec2(0.0, 1.0/uScreenSize.y))*uScreenSize)).r;

    float e = (pow(dSamples[0]-dSamples[1], 2.0) + pow(dSamples[0]-dSamples[2], 2.0) +
               pow(dSamples[0]-dSamples[3], 2.0) + pow(dSamples[0]-dSamples[4], 2.0))*1000000.0f;

    float a = pow(0.99-((uTime/20)-int(uTime/20)), 1.0);
    float b = 1.0f-pow(max((dSamples[0]+0.009-1.0f)*150.0f + 1.0f, 0.0f), 2);
    //vec3 c = t.rgb;
    //fColor = vec4(vec3(b), 1.0);
	fColor = vec4(a*b*((vec3(e*0.5, e, e*0.5)*pow(1.0f-dSamples[0], 0.5))+color.rgb), 1.0);
}
