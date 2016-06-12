#ifndef LIGHTS_H
#define LIGHTS_H

#include "GLObjects.h"

#include <Eigen/Dense>

class Spotlight {
public:
	struct Def {
		Eigen::Vector3f color = {1.0f, 1.0f, 1.0f};
		float coneAngle = M_PI / 8;
		float intensity = 1.0f;
		float smoothness = 1.0f;
		int mapSize = 512;
		float nearClip = 1.0f;
		float farClip = 512.0f;
	};
	Spotlight(Eigen::Vector3f const& position, Eigen::Vector3f const& direction,
			  Def const& def);

	Spotlight(Spotlight const&) = delete;
	Spotlight(Spotlight&&) = default;

	GL::Texture& shadowMap() { return m_shadowMap; }
	Eigen::Matrix4f const& worldToClip() const { return m_worldToClip; }

	void updateTransform();

	Eigen::Vector3f position;
	Eigen::Vector3f direction;
	Eigen::Vector3f color;
	float coneAngle;
	float intensity;
	float smoothness;
	float nearClip;
	float farClip;
private:
	GL::Texture m_shadowMap;
	Eigen::Matrix4f m_worldToClip;
};

#endif // LIGHTS_H
