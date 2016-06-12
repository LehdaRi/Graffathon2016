#ifndef DEPTHVISITOR_HPP
#define DEPTHVISITOR_HPP


#include "Visitor.hpp"
#include "SpotlightComponent.hpp"
#include <vector>


class DepthVisitor : public Visitor<DepthVisitor, SpotlightComponent> {
public:
    void operator()(SpotlightComponent& component);

	std::vector<GLuint> depthTexUints;
};


#endif // DEPTHVISITOR_HPP
