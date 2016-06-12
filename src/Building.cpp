#include "Building.hpp"
#include "Scene.hpp"
#include "MeshComponent.hpp"
#include "TransformationComponent.hpp"
#include "GLUtils.h"


std::default_random_engine Building::r__(715517);
std::unordered_map<uint64_t, std::vector<Mesh>> Building::blocks__;


using namespace Eigen;

//  BuildingFloor

BuildingFloor::BuildingFloor(std::default_random_engine& r, const NodeId& bRoot, int size) :
    root_(SCENE.addNode(bRoot))
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, bRoot);
    tc.translate(Vector3f(0.0f, 0.0f, 0.0f));


    uint32_t ca = 4 + 2*(r()%2);
    uint32_t cb = 4 + 2*(r()%2);
    uint32_t cc, cd, ce, cf;

    rooms_.push_back(SCENE.addNode(bRoot));
    auto& c1n = rooms_.back();
    SCENE.addComponent<MeshComponent>(c1n, c1n, &Building::getRandomBlock(Building::makeBlockGroupId(1, 0, 0, ca, cb)));
    auto& tc1 = SCENE.addComponent<TransformationComponent>(c1n, root_);
    tc1.rotateY(PI);
    tc1.translate(Vector3f(0.0f-size/2, 0.0f, 0.0f));

    uint32_t a[3] = {4 + 2*(r()%2), 4 + 4*(r()%2), ca};

    int x = 2;
    while (x < size) {
        x += a[1]/2;

        rooms_.push_back(SCENE.addNode(bRoot));
        auto& n = rooms_.back();

        SCENE.addComponent<MeshComponent>(n, n, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, a[0], a[1], a[2])));
        auto& tc = SCENE.addComponent<TransformationComponent>(n, root_);
        tc.rotateY(3*PI/2);
        tc.translate(Vector3f(x-size/2, 0.0f, 0.0f));

        x += a[1]/2;
        a[2] = a[0];
        a[0] = 4 + 2*(r()%2);
        a[1] = 4 + 4*(r()%2);
    }
    x += 2;
    cc = a[2];
    cd = 4 + 2*(r()%2);
    ce = 4 + 2*(r()%2);

    rooms_.push_back(SCENE.addNode(bRoot));
    auto& c2n = rooms_.back();
    SCENE.addComponent<MeshComponent>(c2n, c2n, &Building::getRandomBlock(Building::makeBlockGroupId(1, 0, 0, cd, cc)));
    auto& tc2 = SCENE.addComponent<TransformationComponent>(c2n, root_);
    tc2.rotateY(3*PI/2);
    tc2.translate(Vector3f(x-size/2, 0.0f, 0.0f));

    rooms_.push_back(SCENE.addNode(bRoot));
    auto& c3n = rooms_.back();
    SCENE.addComponent<MeshComponent>(c3n, c3n, &Building::getRandomBlock(Building::makeBlockGroupId(1, 0, 0, ce, cd)));
    auto& tc3 = SCENE.addComponent<TransformationComponent>(c3n, root_);
    tc3.translate(Vector3f(x-size/2, 0.0f, -4.0f));

    x-=2;
    a[0] = 4 + 2*(r()%2);
    a[1] = 4 + 4*(r()%2);
    a[2] = ce;
    while (x > 2) {
        if (x == 6)
            a[1] = 4;
        x -= a[1]/2;

        rooms_.push_back(SCENE.addNode(bRoot));
        auto& n = rooms_.back();

        SCENE.addComponent<MeshComponent>(n, n, &Building::getRandomBlock(Building::makeBlockGroupId(0, 0, a[0], a[1], a[2])));
        auto& tc = SCENE.addComponent<TransformationComponent>(n, root_);
        tc.rotateY(PI/2);
        tc.translate(Vector3f(x-size/2, 0.0f, -4.0f));

        x -= a[1]/2;
        a[2] = a[0];
        a[0] = 4 + 2*(r()%2);
        a[1] = 4 + 4*(r()%2);
    }
    cf = a[2];

    rooms_.push_back(SCENE.addNode(bRoot));
    auto& c4n = rooms_.back();
    SCENE.addComponent<MeshComponent>(c4n, c4n, &Building::getRandomBlock(Building::makeBlockGroupId(1, 0, 0, ca, cf)));
    auto& tc4 = SCENE.addComponent<TransformationComponent>(c4n, root_);
    tc4.rotateY(PI/2);
    tc4.translate(Vector3f(0.0f-size/2, 0.0f, -4.0f));
}

BuildingFloor::BuildingFloor(const BuildingFloor& other, const NodeId& bRoot,
                             const Eigen::Vector3f& translation) :
    root_(SCENE.addNode(bRoot))
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, bRoot);
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
{
    other.root_ = NodeId();
    other.rooms_.clear();
}

BuildingFloor::~BuildingFloor(void) {
    SCENE.deleteNode(root_);
}

//  Building

Building::Building(int size, int nFloors, const Vector3f& pos) :
    root_   (SCENE.addNode())
{
    auto& tc = SCENE.addComponent<TransformationComponent>(root_, NodeId());
    tc.translate(pos);
    floors_.emplace_back(r__, root_, size);
    for (auto i=1; i<nFloors; ++i)
    floors_.emplace_back(floors_.front(), root_, Vector3f(0.0f, i*2.0f, 0.0f));
}

Building::Building(Building&& other) :
    root_(std::move(other.root_)),
    floors_(std::move(other.floors_))
{
    other.root_ = NodeId();
    other.floors_.clear();
}

Building::~Building(void) {
    SCENE.deleteNode(root_);
}

void Building::loadBlockMeshes(void) {
    //  sides
    blocks__[makeBlockGroupId(0, 0, 4, 4, 4)].emplace_back("res/block_side_4_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 4, 6)].emplace_back("res/block_side_4_4_6_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 4, 4)].emplace_back("res/block_side_6_4_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 4, 6)].emplace_back("res/block_side_6_4_6_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 8, 4)].emplace_back("res/block_side_4_8_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 4, 8, 6)].emplace_back("res/block_side_4_8_6_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 8, 4)].emplace_back("res/block_side_6_8_4_a.obj");
    blocks__[makeBlockGroupId(0, 0, 6, 8, 6)].emplace_back("res/block_side_6_8_6_a.obj");
    //  corners
    blocks__[makeBlockGroupId(1, 0, 0, 4, 4)].emplace_back("res/block_corner_4_4_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 4, 6)].emplace_back("res/block_corner_4_6_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 4)].emplace_back("res/block_corner_6_4_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 4)].emplace_back("res/block_corner_6_4_b.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 6)].emplace_back("res/block_corner_6_6_a.obj");
    blocks__[makeBlockGroupId(1, 0, 0, 6, 6)].emplace_back("res/block_corner_6_6_b.obj");
    //  inv. corners
    blocks__[makeBlockGroupId(2, 4, 4, 4, 4)].emplace_back("res/block_invcorner_4_4_4_4_a.obj");
    blocks__[makeBlockGroupId(2, 4, 4, 6, 6)].emplace_back("res/block_invcorner_4_4_6_6_a.obj");
    blocks__[makeBlockGroupId(2, 4, 6, 6, 4)].emplace_back("res/block_invcorner_4_6_6_4_a.obj");
    blocks__[makeBlockGroupId(2, 6, 6, 4, 4)].emplace_back("res/block_invcorner_6_6_4_4_a.obj");
    blocks__[makeBlockGroupId(2, 6, 6, 6, 6)].emplace_back("res/block_invcorner_6_6_6_6_a.obj");
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
