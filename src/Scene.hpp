#ifndef SCENE_HPP
#define SCENE_HPP


#include "Node.hpp"
#include "Visitor.hpp"

#include <map>


#define SCENE Scene::getInstance()


class Scene {
public:
    friend class NodeId;

    //  Scene is singleton class due to static component lists in accessComponents method
    static Scene& getInstance(void);
    Scene(const Scene&)             = delete;
    Scene(Scene&&)                  = delete;
    Scene& operator=(const Scene&)  = delete;
    Scene& operator=(Scene&&)       = delete;

    //void reserveNodes(unsigned nReservedNodes);

    //  add top-level node
    NodeId addNode(void);
    //  add child node
    NodeId addNode(const NodeId& parent);

    void deleteNode(const NodeId& nodeId);
    uint64_t getNodesNumber(int32_t level = -1);

    void printNodes(void);  //TEMP

    template <typename T_Component, typename... Args>
    T_Component& addComponent(const NodeId& node, Args&&... args);

    template <typename T_Visitor, typename T_Component>
    void operator()(Visitor<T_Visitor, T_Component>& visitor);

private:
    Scene(void);

    struct NodeLevel {
        std::vector<Node> nodes;
        int64_t firstFreeId;

        NodeLevel(void) : firstFreeId(-1) {}
    };

    inline static void updateFirstFreeId(NodeLevel& nodeLevel);

    static std::map<uint32_t, NodeLevel> nodes_;
    int64_t nTotalNodes;

    //  invalidation frees the node id and invalidates & deletes children
    void invalidateNode(Node& node);


    //  access component data structures
    template <typename T_Component>
    std::map<uint32_t, std::vector<T_Component>>& accessComponents(void);
    template <typename T_Component>
    inline std::vector<T_Component>& accessComponents(uint32_t level);

    //  first free ids using type id system. unordered_map for type ids, map for levels
    std::unordered_map<uint32_t, std::map<uint32_t, int64_t>> componentFirstFreeIds_;

    template <typename T_Component>
    void updateComponentPointers(void);
};


template <typename T_Component, typename... Args>
T_Component& Scene::addComponent(const NodeId& node, Args&&... args) {
    auto& cv = accessComponents<T_Component>(node.level_);

    //  type Id
    auto typeId = ComponentBase::getTypeId<T_Component>();
    //  level map of first free ids
    auto& ffIdLevel = componentFirstFreeIds_[typeId];
    //  if specified level has no components, initialize first free id to -1
    if (ffIdLevel.find(node.level_) == ffIdLevel.end())
        ffIdLevel[node.level_] = -1;

    auto& ffId = ffIdLevel[node.level_];

    if (ffId == -1) {   //  no invalidated node slots available
        auto cap1 = cv.capacity();
        cv.emplace_back(std::forward<Args>(args)...);
        auto cap2 = cv.capacity();

        (*node).setComponent<T_Component>(&cv.back(), cv.size()-1);

        //  if capacity is increased(vector potentially moved in memory),
        //  component pointers stored in nodes need to be updated
        if (cap2 > cap1)
            updateComponentPointers<T_Component>();

        return cv.back();
    }
    else {
        T_Component& c = cv[ffId];
        c = std::move(T_Component(std::forward<Args>(args)...));

        (*node).setComponent<T_Component>(&c, ffId);

        //  find next free id
        ffId = -1;
        for (auto i=ffId+1; i<(int64_t)cv.size(); ++i) {
            if (!cv[i].valid_) {
                ffId = i;
                break;
            }
        }

        return c;
    }
}

template <typename T_Visitor, typename T_Component>
void Scene::operator()(Visitor<T_Visitor, T_Component>& visitor) {
    auto& components = accessComponents<T_Component>();

    for (auto& cv : components)
        for (auto& c : cv.second)
            if (c.valid_ && c.active_)
                visitor(c);
}

template <typename T_Component>
std::map<uint32_t, std::vector<T_Component>>& Scene::accessComponents(void) {
    //  similar to nodes_ data structure, components are also ordered by levels
    static std::map<uint32_t, std::vector<T_Component>> components;
    return components;
}

template <typename T_Component>
inline std::vector<T_Component>& Scene::accessComponents(uint32_t level) {
    return accessComponents<T_Component>()[level];
}

template <typename T_Component>
void Scene::updateComponentPointers(void) {
    uint32_t l = 0;

    for (auto& nl : nodes_) {
        auto& cv = accessComponents<T_Component>(l++);

        for (auto& n : nl.second.nodes)
            if (n.hasComponent<T_Component>())
                n.updateComponentPointer(cv);
    }
}


#endif  //  SCENE_HPP
