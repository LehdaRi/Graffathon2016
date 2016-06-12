#include "SpotlightVisitor.h"

#include "GLFunctions.h"
#include "Renderer.hpp"
#include "Scene.hpp"

void SpotlightVisitor::operator()(SpotlightComponent& component)
{
	component.updateTransform();

	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GL::clear(GL_DEPTH_BUFFER_BIT, component.m_fbo);

	Renderer renderer(component.m_camera, component.m_shadowMap.width_, component.m_shadowMap.height_, component.m_shader, component.m_fbo);

	SCENE(renderer);
}
