#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP


#include "ComponentBase.hpp"
#include "Mesh.h"


class MeshComponent : public ComponentBase {
public:
    friend class Renderer;

    MeshComponent(Mesh* mesh);

private:
    Mesh* mesh_;
};


#endif // MESHCOMPONENT_HPP
