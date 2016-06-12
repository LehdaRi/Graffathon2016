#include "SpotlightComponent.hpp"

#include "GLUtils.h"
#include "TransformationComponent.hpp"
#include "Scene.hpp"

SpotlightComponent::SpotlightComponent(const NodeId& node, const SpotlightDef& def) :
	color(def.color),
	coneAngle(def.coneAngle),
	intensity(def.intensity),
	smoothness(def.smoothness),
	nearClip(def.nearClip),
	farClip(def.farClip),
	m_node(node),
	m_shadowMap(GL::Texture::empty_2D_depth(def.mapSize, def.mapSize))
{
	auto parent = m_node.ref().getParent();
	if (!parent.ref().hasComponent<TransformationComponent>())
		parent = NodeId();

	// We store the camera's position matrix, thus inverse.
	// The function look_at produces world2view matrices.
	SCENE.addComponent<TransformationComponent>(m_node,
	                                            parent,
	                                            GLUtils::look_at(def.position, def.position + def.direction).inverse());
	updateTransform();
}

void SpotlightComponent::updateTransform()
{
	const auto& cumulativeInverse = m_node.ref().getComponent<TransformationComponent>().getTransformCumulative();

	m_worldToClip = GLUtils::perspective(1.0f, 1.0f, coneAngle, nearClip, farClip) * cumulativeInverse.inverse();
}
