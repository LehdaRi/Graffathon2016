#include "Node.hpp"


Node::Node(uint64_t id, uint32_t level, const NodeId& parent) :
    id_(id),
    level_(level),
    valid_(std::make_shared<bool>(true)),
    active_(true),
    parent_(parent)
{}

bool Node::isValid(void) const {
    return *valid_;
}

NodeId Node::getId(void) const {
    return NodeId(id_, level_, valid_);
}

unsigned Node::getChildrenNumber(void) const {
    return children_.size();
}

void Node::print(void) {
    for (auto i=0u; i<level_; ++i)
        printf("  ");

    printf("%u,%llu\n", level_, id_);

    for (auto& c : children_)
        if (c)
            (*c).print();
}

void Node::addChild(const NodeId& nodeId) {
    children_.push_back(nodeId);
}
/*
void Node::invalidateComponents(void) {
    for (auto& c : components_) {
        c.second.component->invalidate();
        c.second.component = nullptr;
        c.second.id = -1;
    }
}*/
