#include "Building.hpp"
#include "Scene.hpp"
#include "MeshComponent.hpp"
#include "TransformationComponent.hpp"


std::default_random_engine Building::r__(715517);
std::unordered_map<uint64_t, std::vector<Mesh>> Building::blocks__;


using namespace Eigen;


Building::Building(void) :
    root_   (SCENE.addNode())
{
    floors_.emplace_back(r__, root_);
}

Building::~Building(void) {
    SCENE.deleteNode(root_);
}

void Building::loadBlockMeshes(void) {
    blocks__[makeBlockGroupId(0, 0, 4, 4, 4)].emplace_back("res/block_side_4_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 4, 6)].emplace_back("res/block_side_4_4_6_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 8, 4)].emplace_back("res/block_side_4_8_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 4, 4)].emplace_back("res/block_side_6_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 4, 6)].emplace_back("res/block_side_6_4_6_a.obj");
}

uint64_t Building::makeBlockGroupId(uint8_t type,
                                    uint8_t wRight, uint8_t wTop,
                                    uint8_t wLeft, uint8_t wBottom) {
    return (uint64_t)type | (uint64_t)wRight << 8 | (uint64_t)wTop << 16 | (uint64_t)wLeft << 24 | (uint64_t)wBottom << 32;
}

Mesh& Building::getRandomBlock(uint64_t groupId) {
    auto& v = blocks__[groupId];
    return v[r__() % v.size()];
}



BuildingFloor::BuildingFloor(std::default_random_engine& r, const NodeId& bRoot) :
    root_(SCENE.addNode(bRoot))
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, bRoot);
    tc.translate(Vector3f(-10.0f, 0.0f, 0.0f));

    for (auto i=0u; i<5; ++i) {
        rooms_.push_back(SCENE.addNode(bRoot));
        auto& rn = rooms_.back();
        switch (r()%5) {
            case 0:
            SCENE.addComponent<MeshComponent>(rn, rn, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, 4, 4, 4)));
            break;
            case 1:
            SCENE.addComponent<MeshComponent>(rn, rn, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, 4, 4, 6)));
            break;
            case 2:
            SCENE.addComponent<MeshComponent>(rn, rn, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, 4, 8, 4)));
            break;
            case 3:
            SCENE.addComponent<MeshComponent>(rn, rn, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, 6, 4, 4)));
            break;
            case 4:
            SCENE.addComponent<MeshComponent>(rn, rn, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, 4, 4, 6)));
            break;
        }
        auto& tc = SCENE.addComponent<TransformationComponent>(rn, root_);
        tc.translate(Vector3f(r()%5, r()%5, r()%5));
    }
}
