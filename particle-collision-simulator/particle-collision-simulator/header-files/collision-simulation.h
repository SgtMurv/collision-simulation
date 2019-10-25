
#include <vector>
#include "particle.h"



class CollisionSimulation : public Application
{
    float g = 10.0f;
    std::vector<Particle> particles;
public:
    CollisionSimulation();
    virtual void display();
    virtual void update();
    void box_collision_resolve(Particle &particle);
    bool out_of_the_box_test(Particle particle);
    void out_of_the_box_resolve(Particle &particle);
};
