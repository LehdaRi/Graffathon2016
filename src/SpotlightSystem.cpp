#include "SpotlightSystem.hpp"

#include "Scene.hpp"
#include "SpotlightComponent.hpp"

// SpotlightSystem

SpotlightSystem::SpotlightSystem(void) :
	m_lightNode0 (SCENE.addNode())
{
	SpotlightDef def;
	def.position  = {0.0f, 15.0f, 55.0f};
	def.direction = -def.position;
	SCENE.addComponent<SpotlightComponent>(m_lightNode0, m_lightNode0, def);
}

SpotlightSystem::~SpotlightSystem(void)
{
	SCENE.deleteNode(m_lightNode0);
}
