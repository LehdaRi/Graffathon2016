#include "Building.hpp"
#include "Scene.hpp"
#include "MeshComponent.hpp"
#include "TransformationComponent.hpp"


std::default_random_engine Building::r__(715517);
std::unordered_map<uint64_t, std::vector<Mesh>> Building::blocks__;


using namespace Eigen;

//  BuildingFloor

BuildingFloor::BuildingFloor(std::default_random_engine& r, const NodeId& bRoot) :
    root_(SCENE.addNode(bRoot))
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, bRoot);
    tc.translate(Vector3f(0.0f, 0.0f, 0.0f));

    for (auto i=0u; i<4; ++i) {
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
        tc.translate(Vector3f((r()%1001)*0.01f-5.0f, 0, (r()%101)*0.001f-5.0f));
    }
}

BuildingFloor::BuildingFloor(const BuildingFloor& other, const NodeId& bRoot,
                             const Eigen::Vector3f& translation) :
    root_(SCENE.addNode(bRoot))
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, NodeId());
    tc.translate(translation);

    for (auto& rr : other.rooms_) {
        rooms_.push_back(SCENE.addNode(bRoot));
        auto& rn = rooms_.back();
        SCENE.addComponent<MeshComponent>(rn, rn, rr.ref().getComponent<MeshComponent>());
        SCENE.addComponent<TransformationComponent>(rn, root_, rr.ref().getComponent<TransformationComponent>());
    }
}

BuildingFloor::BuildingFloor(BuildingFloor&& other) :
    root_(std::move(other.root_)),
    rooms_(std::move(other.rooms_))
{}

BuildingFloor::~BuildingFloor(void) {
    //SCENE.deleteNode(root_);
}

//  Building

Building::Building(void) :
    root_   (SCENE.addNode())
{
    floors_.emplace_back(r__, root_);
    for (auto i=1; i<15; ++i)
    floors_.emplace_back(floors_.front(), root_, Vector3f(0.0f, i*2.0f, 0.0f));
}

Building::~Building(void) {
    SCENE.deleteNode(root_);
}

void Building::loadBlockMeshes(void) {
    //  sides
    blocks__[makeBlockGroupId(0, 0, 4, 4, 4)].emplace_back("res/block_side_4_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 4, 6)].emplace_back("res/block_side_4_4_6_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 8, 4)].emplace_back("res/block_side_4_8_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 4, 4)].emplace_back("res/block_side_6_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 4, 6)].emplace_back("res/block_side_6_4_6_a.obj");
    //  corners
    blocks__[makeBlockGroupId(1, 0, 0, 4, 4)].emplace_back("res/block_corner_4_4_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 4, 4)].emplace_back("res/block_corner_4_4_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 4)].emplace_back("res/block_corner_6_4_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 4)].emplace_back("res/block_corner_6_4_b.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 6)].emplace_back("res/block_corner_6_6_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 6)].emplace_back("res/block_corner_6_6_b.obj");
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
