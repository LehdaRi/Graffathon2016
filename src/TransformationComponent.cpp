#include "TransformationComponent.hpp"
#include "Node.hpp"


using namespace Eigen;


std::default_random_engine TransformationComponent::r__;


TransformationComponent::TransformationComponent(const NodeId& parent, const Matrix4f& m) :
    parent_((parent && parent.ref().hasComponent<TransformationComponent>()) ? parent : NodeId()),
    m_  (m),
    s_  {(r__()%10000)*0.000628318531, (r__()%10000)*0.000628318531, (r__()%10000)*0.000628318531}
{}

TransformationComponent::TransformationComponent(const NodeId& parent,
                                                 const TransformationComponent& other) :
    parent_((parent && parent.ref().hasComponent<TransformationComponent>()) ? parent : NodeId()),
    m_  (other.m_),
    s_  {other.s_[0], other.s_[1], other.s_[2]}
{}

void TransformationComponent::reset(void) {
    m_ = Matrix4f::Identity();
}

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
