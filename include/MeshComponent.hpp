#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP


#include "ComponentBase.hpp"
#include "Mesh.h"


class MeshComponent : public ComponentBase {
public:
    MeshComponent(Mesh& mesh);

private:
    Mesh& mesh_;
};


#endif // MESHCOMPONENT_HPP
