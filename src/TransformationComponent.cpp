#include "TransformationComponent.hpp"
#include "Node.hpp"

#include <iostream>


using namespace Eigen;


TransformationComponent::TransformationComponent(const NodeId& parent, const Matrix4f& m) :
    parent_((parent && parent.ref().hasComponent<TransformationComponent>()) ? parent : NodeId()),
    m_  (m)
{
    printf("%u %u\n", (bool)parent, (bool)parent_);
}

void TransformationComponent::translate(const Vector3f& v) {
    Matrix4f tm;
    tm <<   Matrix3f::Identity(),           v,
            Vector3f::Zero().transpose(),   1.0f;

    m_ = tm * m_;
}
