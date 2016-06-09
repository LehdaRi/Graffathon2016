#include "Scene.hpp"


#include <chrono>
#include <iostream>
#include <deque>


std::map<uint32_t, Scene::NodeLevel> Scene::nodes_;


Scene& Scene::getInstance(void) {
    static Scene instance;
    return instance;
}
/*
void Scene::reserveNodes(unsigned nReservedNodes) {
    nodes_.reserve(nReservedNodes);
}
*/
NodeId Scene::addNode(void) {
    nTotalNodes = -1;
    auto& nl = nodes_[0];

    //  if no free invalidated nodes available, create a new one
    if (nl.firstFreeId == -1) {
        nl.nodes.emplace_back(nl.nodes.size());
        return nl.nodes.back().getId();
    }
    else {  //  else use first invalidated node
        Node& node = nl.nodes[nl.firstFreeId];
        node.valid_.reset(new bool(true));
        node.active_ = true;

        updateFirstFreeId(nl);
        return node.getId();
    }
}

NodeId Scene::addNode(const NodeId& parent) {
    nTotalNodes = -1;
    auto& nl = nodes_[parent.level_+1];

    if (nl.firstFreeId == -1) {
        nl.nodes.emplace_back(nl.nodes.size(), parent.level_+1);

        auto newNodeId = nl.nodes.back().getId();
        (*parent).addChild(newNodeId);

        return newNodeId;
    }
    else {
        Node& node = nl.nodes[nl.firstFreeId];
        node.valid_.reset(new bool(true));
        node.active_ = true;
        node.parent_ = parent;

        auto newNodeId = node.getId();
        (*parent).addChild(newNodeId);

        updateFirstFreeId(nl);
        return newNodeId;
    }
}

void Scene::deleteNode(const NodeId& nodeId) {
    //  check if nodeId is valid
    if (!nodeId)
        return;

    nTotalNodes = -1;

    auto level = nodeId.level_;
    auto id = nodeId.id_;

    invalidateNode(*nodeId);

    if (nodes_[level].firstFreeId == -1 || nodes_[level].firstFreeId > (int64_t)id)
        nodes_[level].firstFreeId = id;
}

uint64_t Scene::getNodesNumber(int32_t level) {
    if (level == -1) {
        if (nTotalNodes == -1) {
            nTotalNodes = 0;
            for (auto& nl : nodes_) {
                for (auto& n : nl.second.nodes)
                    if (n.isValid())
                        nTotalNodes++;
            }
        }
        return nTotalNodes;
    }
    else
        return nodes_[level].nodes.size();
}

void Scene::printNodes(void) {
    for (auto& node : nodes_[0].nodes)
        node.print();
}

Scene::Scene(void) :
    nTotalNodes(-1)
{}

void Scene::updateFirstFreeId(NodeLevel& nodeLevel) {
    if (nodeLevel.firstFreeId == -1)
        nodeLevel.firstFreeId = 0;

    for (auto i=nodeLevel.firstFreeId; i<(int64_t)nodeLevel.nodes.size(); ++i) {
        if (!*(nodeLevel.nodes[i].valid_)) {
            nodeLevel.firstFreeId = i;
            return;
        }
    }

    nodeLevel.firstFreeId = -1;
}

void Scene::invalidateNode(Node& node) {
    //  invalidate components of node
    for (auto& c : node.components_) {
        auto& ffId = componentFirstFreeIds_[c.first][node.level_];

        c.second.component->invalidate();
        //  update first free id
        if (ffId == -1 || c.second.id < ffId)
            ffId = c.second.id;

        c.second.component = nullptr;
        c.second.id = -1;
    }
    node.components_.clear();

    //  delete children
    for (auto& c : node.children_)
        deleteNode(c);

    node.children_.clear();

    //  invalidate
    *node.valid_ = false;
    node.active_ = false;
}
