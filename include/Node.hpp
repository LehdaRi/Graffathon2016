#ifndef NODE_HPP
#define NODE_HPP


#include <unordered_map>
#include <vector>
#include <memory>

#include "NodeId.hpp"
#include "ComponentBase.hpp"


class Node {
public:
    friend class Scene;

    Node(uint64_t id, uint32_t level = 0u, const NodeId& parent = NodeId());

    bool isValid(void) const;
    NodeId getId(void) const;
    unsigned getChildrenNumber(void) const;

    void print(void);    //TEMP

    template <typename T_Component>
    bool hasComponent(void) const;
    template <typename T_Component>
    T_Component& getComponent(void);

private:
    uint64_t id_;
    uint32_t level_;
    std::shared_ptr<bool> valid_;
    bool active_;

    NodeId parent_;
    std::vector<NodeId> children_;

    //  adding children happens through Scene, therefore addChild is a private function
    void addChild(const NodeId& nodeId);

    struct ComponentInfo {
        ComponentBase* component;
        int64_t id;    //  location on the component vector, required for updating the pointer

        ComponentInfo() : component(nullptr), id(-1) {}
    };

    //  Component infos stored with typeId
    std::unordered_map<uint32_t, ComponentInfo> components_;

    template <typename T_Component>
    void setComponent(ComponentBase* component, uint64_t id);

    template <typename T_Component>
    void updateComponentPointer(std::vector<T_Component>& cv);

    void invalidateComponents(void);
};


template <typename T_Component>
bool Node::hasComponent(void) const {
    return components_.find(ComponentBase::getTypeId<T_Component>()) != components_.end();
}

template <typename T_Component>
T_Component& Node::getComponent(void) {
    return *static_cast<T_Component*>(components_.at(ComponentBase::getTypeId<T_Component>()).component);
}

template <typename T_Component>
void Node::setComponent(ComponentBase* component, uint64_t id) {
    auto& c = components_[ComponentBase::getTypeId<T_Component>()];
    c.component = component;
    c.id = id;
}

template <typename T_Component>
void Node::updateComponentPointer(std::vector<T_Component>& cv) {
    auto& c = components_[ComponentBase::getTypeId<T_Component>()];
    c.component = &cv[c.id];
}


#endif  //  NODE_HPP
