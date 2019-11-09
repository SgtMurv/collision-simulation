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

class QuadTree {
private:
    float x;
    float y;
    float width;
    float height;
    int level;
    int maxLevel;
    std::vector<Particle*>particles;
    QuadTree *parent;
    QuadTree *NW;
    QuadTree *NE;
    QuadTree *SW;
    QuadTree *SE;
    bool contains(QuadTree *child, Particle *particle);
public:
    QuadTree(float x, float y, float width, float height, int level, int maxLevel);
    ~QuadTree();
    
    // testing method to print out the level of the quad tree and recursively call the branches of itself.
    void printDetails();
    
    
    
    
    void addParticle(Particle *particle);
    void getPotentialCollidingParticles(std::vector<std::vector<Particle*>>& potentialCollisions);
    void clearParticles();
    // I assume this is used for dynamic trees:
    int size;
};
