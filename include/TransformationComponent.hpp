#ifndef TRANSFORMATIONCOMPONENT_HPP
#define TRANSFORMATIONCOMPONENT_HPP


#include "NodeId.hpp"
#include "ComponentBase.hpp"

#include <Eigen/Dense>


class TransformationComponent : public ComponentBase {
public:
    friend class TransformationVisitor;
    friend class Renderer;
    friend class SpotlightRenderer;

    TransformationComponent(const NodeId& parent,
                            const Eigen::Matrix4f& m = Eigen::Matrix4f::Identity());
    TransformationComponent(const NodeId& parent,
                            const TransformationComponent& other);

    void translate (const Eigen::Vector3f& v);
    void rotateY   (float angle);

	const Eigen::Matrix4f& getTransformRelative   (void) const { return m_; }
	const Eigen::Matrix4f& getTransformCumulative (void) const { return mCumulative_; }

private:
    NodeId parent_;
    Eigen::Matrix4f m_;
    Eigen::Matrix4f mCumulative_;
};


#endif // TRANSFORMATIONCOMPONENT_HPP
