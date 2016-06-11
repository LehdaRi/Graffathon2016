#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP


#include "ComponentBase.hpp"
#include "NodeId.hpp"
#include "Mesh.h"


class MeshComponent : public ComponentBase {
public:
    friend class Renderer;

    MeshComponent(const NodeId& node, Mesh* mesh);

private:
    NodeId node_;
    Mesh* mesh_;
};


#endif // MESHCOMPONENT_HPP
