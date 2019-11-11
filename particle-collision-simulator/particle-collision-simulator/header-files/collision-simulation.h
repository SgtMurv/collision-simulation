#pragma once
#define GL_SILENCE_DEPRECATION // this line silences the yellow warnings of deprecation of OpenGL from
#include <vector>
#include <math.h>
#include <GLUT/GLUT.h>
#include "particle.h"
#include "particle-world.h"
#include "coreMath.h"
#include "quad-tree.h"

const Vector2 Vector2::GRAVITY = Vector2(0,-9.81);

class CollisionSimulation : public Application
{
    std::vector<Particle*> particles;
    std::vector<Platform*> platforms;
    ParticleWorld world;
public:
    CollisionSimulation();
    ~CollisionSimulation();
    virtual void display();
    virtual void update();
    void drawLine(Vector2* start, Vector2* end);
    void drawPartitions(float nRange);
};
