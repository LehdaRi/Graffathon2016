#include "ComponentBase.hpp"


unsigned ComponentBase::typeIdCounter__ = 0u;


ComponentBase::ComponentBase(void) :
    valid_(true), active_(true)
{}

void ComponentBase::invalidate(void) {
    valid_ = false;
    active_ = false;
}
