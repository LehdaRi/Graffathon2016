#include "DepthVisitor.hpp"

void DepthVisitor::operator()(SpotlightComponent& component)
{
	depthTexUints.push_back(component.shadowMap());
}
