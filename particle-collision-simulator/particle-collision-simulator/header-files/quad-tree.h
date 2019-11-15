#pragma once
#include <list>
#include <vector>
#include <iostream>
#include "../header-files/coreMath.h"
#include "../header-files/particle.h"

class QuadTree {
private:
    float x;
    float y;
    float width;
    float height;
    int level;
    
    std::vector<Particle*>particles;
    QuadTree *NW;
    QuadTree *NE;
    QuadTree *SW;
    QuadTree *SE;
public:
    int maxLevel;
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
