#include "TransformationComponent.hpp"
#include "Node.hpp"


using namespace Eigen;


TransformationComponent::TransformationComponent(const NodeId& parent, const Matrix4f& m) :
    parent_((parent && parent.ref().hasComponent<TransformationComponent>()) ? parent : NodeId()),
    m_  (m)
{}

TransformationComponent::TransformationComponent(const NodeId& parent,
                                                 const TransformationComponent& other) :
    parent_((parent && parent.ref().hasComponent<TransformationComponent>()) ? parent : NodeId()),
    m_  (other.m_)
{}

void TransformationComponent::translate(const Vector3f& v) {
    Matrix4f tm;
    tm <<   Matrix3f::Identity(),           v,
            Vector3f::Zero().transpose(),   1.0f;

    m_ = tm * m_;
}

void TransformationComponent::rotateY(float angle) {
    Matrix4f rm;
    rm <<   cosf(angle),    0.0f,   sinf(angle),    0.0f,
            0.0f,           1.0f,   0.0f,           0.0f,
            -sinf(angle),   0.0f,   cosf(angle),    0.0f,
            0.0f,           0.0f,   0.0f,           1.0f;
    m_ = rm * m_;
}
