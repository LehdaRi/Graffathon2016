#ifndef BUILDING_HPP
#define BUILDING_HPP


#include "NodeId.hpp"
#include "Mesh.h"
#include <random>
#include <unordered_map>
#include <vector>


class BuildingFloor {
public:
    BuildingFloor(std::default_random_engine& r, const NodeId& bRoot);
    BuildingFloor(const BuildingFloor& other, const NodeId& bRoot,
                  const Eigen::Vector3f& translation);

    BuildingFloor(const BuildingFloor&)             = delete;
    BuildingFloor(BuildingFloor&&);
    BuildingFloor& operator=(const BuildingFloor&)  = delete;
    BuildingFloor& operator=(BuildingFloor&&);

    ~BuildingFloor(void);

private:
    NodeId              root_;
    std::vector<NodeId> rooms_;
};


class Building {
public:
    friend class BuildingFloor;

    Building(void);

    Building(const Building&)               = delete;
    Building(Building&&)                    = delete;
    Building& operator=(const Building&)    = delete;
    Building& operator=(Building&&)         = delete;

    ~Building(void);

    //  call this to preload block meshes
    static void loadBlockMeshes(void);

private:
    NodeId                      root_;
    std::vector<BuildingFloor>  floors_;

    static std::default_random_engine  r__;
    static std::unordered_map<uint64_t, std::vector<Mesh>> blocks__;

    //  type:
    //  0: side, 1: corner, 2: inv.corner
    static uint64_t makeBlockGroupId(uint8_t type, uint8_t wRight, uint8_t wTop, uint8_t wLeft, uint8_t wBottom);
    static Mesh& getRandomBlock(uint64_t groupId);
};


#endif // BUILDING_HPP
