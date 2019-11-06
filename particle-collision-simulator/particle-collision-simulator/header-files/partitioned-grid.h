#pragma once
#include <list>
#include <vector>
#include <iostream>
#include "../header-files/coreMath.h"
#include "../header-files/particle.h"
class PartitionedGrid{
private:
    std::vector<Vector2*> gridAreas;
    std::vector<std::vector<Particle*>> particleGrid;
public:
    void generateGridArray(std::vector<Particle*> particles, float nRange);
    std::vector<std::vector<Particle*>> getParticleGrid();
    void clearDataStructures();
    std::vector<Vector2*> getGridAreas();
};
