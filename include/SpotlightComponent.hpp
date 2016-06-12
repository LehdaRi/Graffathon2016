#ifndef SPOTLIGHTCOMPONENT_H
#define SPOTLIGHTCOMPONENT_H

#include "GLObjects.h"
#include "ComponentBase.hpp"
#include "Node.hpp"
#include "Camera.hpp"
#include "SpotlightRenderer.hpp"


#include <Eigen/Dense>

struct SpotlightDef
{
	Eigen::Vector3f position = {0.0f, 10.0f, 0.0f};
	Eigen::Vector3f direction = {0.0f, -1.0f, 0.0f};
	Eigen::Vector3f color = {1.0f, 1.0f, 1.0f};
	float coneAngle = M_PI / 8;
	float intensity = 1.0f;
	float smoothness = 1.0f;
	int mapSize = 512;
	float nearClip = 1.0f;
	float farClip = 512.0f;
};

class SpotlightComponent : public ComponentBase
{
public:
	friend class SpotlightVisitor;

	SpotlightComponent(const NodeId&, const SpotlightDef& = {});

	GL::Texture& shadowMap() { return m_shadowMap; }
	Eigen::Matrix4f const& worldToClip() const { return m_worldToClip; }

	Eigen::Vector3f color;
	float coneAngle;
	float intensity;
	float smoothness;
	float nearClip;
	float farClip;
private:
	NodeId m_node;
	GL::Texture m_shadowMap;
	GL::Texture m_colorMap;
	GL::FBO m_fbo;

	Camera m_camera;
	Eigen::Matrix4f m_worldToClip;

	GL::ShaderProgram m_shader;

	void updateTransform();
};

#endif // SPOTLIGHTCOMPONENT_H
