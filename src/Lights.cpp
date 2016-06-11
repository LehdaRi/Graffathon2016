#include "Lights.hpp"

#include "GLUtils.h"

Spotlight::Spotlight(Eigen::Vector3f const& position, Eigen::Vector3f const& direction,
                     Def const& def) :
	 position(position),
	 direction(direction),
	 color(def.color),
	 coneAngle(def.coneAngle),
	 intensity(def.intensity),
	 smoothness(def.smoothness),
	 m_shadowMap(GL::Texture::empty_2D_depth(def.mapSize, def.mapSize)),
	 m_worldToClip()
{
	updateTransform();
}

void Spotlight::updateTransform()
{
	m_worldToClip = GLUtils::perspective(1.0f, 1.0f, coneAngle, nearClip, farClip) *
	                GLUtils::look_at(position, position + direction) *
				    Eigen::Affine3f(Eigen::Translation3f(-position)).matrix();
}
