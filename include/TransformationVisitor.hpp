#ifndef TRANSFORMATIONVISITOR_HPP
#define TRANSFORMATIONVISITOR_HPP


#include "Visitor.hpp"
#include "TransformationComponent.hpp"


class TransformationVisitor : public Visitor<TransformationVisitor, TransformationComponent> {
public:
    void operator()(TransformationComponent& component);
};


#endif // TRANSFORMATIONVISITOR_HPP
