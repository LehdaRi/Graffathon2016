#ifndef TRANSFORMATIONVISITOR_HPP
#define TRANSFORMATIONVISITOR_HPP


#include "Visitor.hpp"
#include "TransformationComponent.hpp"


class TransformationVisitor : public Visitor<TransformationVisitor, TransformationComponent> {
public:
    TransformationVisitor();

    void operator()(TransformationComponent& component);

    int mode;
    float magnitude;
    double time;
};


#endif // TRANSFORMATIONVISITOR_HPP
