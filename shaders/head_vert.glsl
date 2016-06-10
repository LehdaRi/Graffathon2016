#version 420

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModelToClip;
uniform mat4 uModelToView;
uniform mat3 uNormalModelToView;

out vec3 vViewPos;
out vec3 vViewNormal;
out vec2 vTexCoord;

void main() {
	vViewPos = (uModelToView * aPosition).xyz;
	vViewNormal = (uNormalModelToView * aNormal).xyz;
	vTexCoord = aTexCoord;

	gl_Position = uModelToClip * aPosition;
}