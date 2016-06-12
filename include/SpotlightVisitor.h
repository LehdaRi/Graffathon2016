#ifndef SPOTLIGHTVISITOR_H
#define SPOTLIGHTVISITOR_H

#include "Visitor.hpp"
#include "SpotlightComponent.hpp"

class SpotlightVisitor : public Visitor<SpotlightVisitor, SpotlightComponent>
{
public:
	void operator()(SpotlightComponent&);
};

#endif // SPOTLIGHTVISITOR_H
