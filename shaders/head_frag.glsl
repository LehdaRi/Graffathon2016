#version 330 core

uniform mat3 uNormalModelToView;
uniform mat4 uViewToWorld;

uniform sampler2D uDiffuseSampler;
uniform sampler2D uNormalSampler;
uniform samplerCube uEnvironmentSampler;

uniform vec3 uSpecularColor;
uniform float uSpecularExponent;
uniform bool uNormalsFromTexture = false;

uniform samplerBuffer uDirLightSampler;
uniform samplerBuffer uPointLightSampler;

int uNumDirLights = textureSize(uDirLightSampler) / 2;
int uNumPointLights = textureSize(uPointLightSampler) / 3;

in vec3 vViewPos;
in vec3 vViewNormal;
in vec2 vTexCoord;

// TEST
uniform bool uUseEnv = false;

layout(location = 0) out vec4 fColor;

void main() {
	vec3 diffuseColor = texture2D(uDiffuseSampler, vTexCoord).xyz;

	vec3 tViewNormal = texture2D(uNormalSampler, vTexCoord).xyz;
	tViewNormal = 2.0 * tViewNormal - vec3(1.0);
	tViewNormal = normalize(uNormalModelToView * tViewNormal);
	vec3 fViewNormal = normalize(vViewNormal);
	vec3 normal = uNormalsFromTexture ? tViewNormal : fViewNormal;
	vec3 viewDir = normalize(-vViewPos);

	if (uUseEnv) {
		vec3 worldNormal = (uViewToWorld * vec4(normal, 0)).xyz;
		vec3 worldViewDir = (uViewToWorld * vec4(viewDir, 0)).xyz;
		const float eta = 1.0 / 1.05;

		
		vec3 refl = reflect(-worldViewDir, worldNormal);
		diffuseColor += 0.25 * texture(uEnvironmentSampler, refl).rgb;
		
		/*
		vec3 refr = refract(-worldViewDir, worldNormal, eta);
		diffuseColor += 0.8 * texture(uEnvironmentSampler, refr).rgb;
		*/
	}

	vec3 color = vec3(0.0);

	for (int i = 0; i < uNumDirLights; ++i) {
		// Light direction & color.
		vec3 lightDir = normalize(texelFetch(uDirLightSampler, 2 * i).xyz);
		vec3 lightCol = texelFetch(uDirLightSampler, 2 * i + 1).xyz;

		// Diffuse coefficient.
		float diffuseCoeff = clamp(dot(normal, lightDir), 0.0, 1.0);

		// Specular coefficient.
		vec3 H = normalize(viewDir + lightDir);
		float specularCoeff = clamp(dot(normal, H), 0.0, 1.0);
		specularCoeff = pow(specularCoeff, uSpecularExponent);

		color += diffuseCoeff * diffuseColor * lightCol;
		color += diffuseCoeff != 0.0 ? specularCoeff * uSpecularColor * lightCol : vec3(0.0);
	}

	for (int i = 0; i < uNumPointLights; ++i) {
		// Light direction, color & attenuation.
		vec3 lightPos = texelFetch(uPointLightSampler, 3 * i).xyz;
		vec3 lightCol = texelFetch(uPointLightSampler, 3 * i + 1).xyz;
		vec3 lightAtt = texelFetch(uPointLightSampler, 3 * i + 2).xyz;

		vec3 lightDir = lightPos - vViewPos;
		float lightDist = length(lightDir);
		lightDir /= lightDist;

		float att = dot(lightAtt, vec3(1, lightDist, lightDist*lightDist));
		lightCol /= att;

		// Diffuse coefficient.
		float diffuseCoeff = clamp(dot(normal, lightDir), 0.0, 1.0);

		// Specular coefficient.
		vec3 H = normalize(viewDir + lightDir);
		float specularCoeff = clamp(dot(normal, H), 0.0, 1.0);
		specularCoeff = pow(specularCoeff, uSpecularExponent);

		color += diffuseCoeff * diffuseColor * lightCol;
		color += diffuseCoeff != 0.0 ? specularCoeff * uSpecularColor * lightCol : vec3(0.0);
	}

	fColor = vec4(color, 1.0);
}