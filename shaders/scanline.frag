/*
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
    vec2 offset = vec2(0.02 * sin(2.0 * M_PI * gl_FragCoord.y / uScreenSize.y * 3 + 20*uTime) * sin(2.0 * M_PI * gl_FragCoord.y / uScreenSize.y * 10 + 10*uTime), 0.0);
	//vec2 offset = vec2(1.0 / uScreenSize.y * gl_FragCoord.y, 0.0);
	fColor = vec4(texture(uScreenTexture, vTexCoord + offset).rgb, 1.0f);
}
*/
#version 330 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D uScreenTexture;
uniform float uTime;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float lines = 300.0;
    vec2 v = vec2(0.5)-vTexCoord;
    float c = length(vec2(0.5) - vTexCoord);

    float d = c * c;
    float n = pow(rand(v + sin(uTime*10.0)), 2) * d * 0.4;

    vec2 vTexCoord2 = vTexCoord * 0.9+0.05 - v*c*0.2;

    vec3 s = vec3(0.9 + 0.3*sin(vTexCoord2.y*3.14*lines),
                  0.9 + 0.3*sin(vTexCoord2.y*3.14*lines + 3.14 * 0.66),
                  0.9 + 0.3*sin(vTexCoord2.y*3.14*lines + 3.14 * 1.33))+0.5-d;

    vec4 t = texture2D(uScreenTexture, vTexCoord2) * vec4(s, 1.0);

    vec4 color = t - t*2*vec4(n, n, n, 1.0) - d*0.3;
    float avg = (color.r + color.g + color.b) / 3;
    vec4 green = vec4(mix(vec3(0.05, 0.05, 0.05), vec3(0.15, 0.75, 0.15), avg), 1.0);
    
    fColor = green;
    //fColor = mix(green, color, 0.3);
}
