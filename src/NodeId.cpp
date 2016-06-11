#include "NodeId.hpp"
#include "Scene.hpp"


NodeId::NodeId(void) :
    valid_(std::make_shared<bool>(false))
{}

NodeId::NodeId(uint64_t id, uint32_t level, const std::shared_ptr<bool>& valid) :
    id_(id), level_(level), valid_(valid)
{}

Node& NodeId::operator*(void) const {
    if(*valid_)
        return SCENE.nodes_[level_].nodes[id_];
    else
        throw "Invalid NodeId";
}

NodeId::operator bool() const {
    return *valid_;
}

Node& NodeId::ref(void) const {
    if(*valid_)
        return SCENE.nodes_[level_].nodes[id_];
    else
        throw "Invalid NodeId";
}
