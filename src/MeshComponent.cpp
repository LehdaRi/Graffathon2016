#include "MeshComponent.hpp"


MeshComponent::MeshComponent(const NodeId& node, Mesh* mesh) :
    node_   (node),
    mesh_   (mesh)
{}

MeshComponent::MeshComponent(const NodeId& node, const MeshComponent& other) :
    node_   (node),
    mesh_   (other.mesh_)
{}
