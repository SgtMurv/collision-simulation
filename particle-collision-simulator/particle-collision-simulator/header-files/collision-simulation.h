#pragma once
#include <vector>
#include "particle.h"
#include "particle-world.h"
#include "coreMath.h"
#include "particle-contact-generator.h"

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
    void box_collision_resolve(Particle &particle);
    bool out_of_the_box_test(Particle particle);
    void out_of_the_box_resolve(Particle &particle);
};
