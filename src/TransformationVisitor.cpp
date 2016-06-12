#include "TransformationVisitor.hpp"
#include "Node.hpp"


using namespace Eigen;


TransformationVisitor::TransformationVisitor() :
    mode(0),
    magnitude(0.0f),
    time(0.0)
{}

void TransformationVisitor::operator()(TransformationComponent& component) {

    Vector3f v(magnitude*sinf(time+component.s_[0]),
               magnitude*sinf(time+component.s_[1]),
               magnitude*sinf(time+component.s_[2]));

    Matrix4f tm;
    tm <<   Matrix3f::Identity(),           v,
            Vector3f::Zero().transpose(),   1.0f;

    if (component.parent_) {
        auto& ptc = component.parent_.ref().getComponent<TransformationComponent>();
        component.mCumulative_ = tm * ptc.mCumulative_ * component.m_;
    }
    else
        component.mCumulative_ = tm * component.m_;
}
