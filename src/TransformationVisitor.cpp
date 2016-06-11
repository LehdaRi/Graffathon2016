#include "TransformationVisitor.hpp"
#include "Node.hpp"


void TransformationVisitor::operator()(TransformationComponent& component) {
    if (component.parent_) {
        auto& ptc = component.parent_.ref().getComponent<TransformationComponent>();
        component.mCumulative_ = ptc.mCumulative_ * component.m_;
    }
    else
        component.mCumulative_ = component.m_;
}
