#include "SpotlightVisitor.h"

#include "GLFunctions.h"
#include "Renderer.hpp"
#include "Scene.hpp"

void SpotlightVisitor::operator()(SpotlightComponent& component)
{
	component.updateTransform();

	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GL::clear(GL_DEPTH_BUFFER_BIT, component.m_fbo);

	SCENE(component.m_renderer);
}
