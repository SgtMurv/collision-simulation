#pragma once
#include "particle.h"
#include "particle-contacts.h"
#include "stationary-object.h"
#include <vector>

// interface class for stationary objects in the simulation
class StationaryObject{
public:
    virtual unsigned checkForContact(ParticleContact *contact, unsigned limit) const = 0;
};

class Platform : public StationaryObject{
private:
    Vector2 start;
    Vector2 end;
    float restitution;
    // list of the moving objects that the platform needs to check collisions with
    std::vector<Particle*> particles;
public:
    void setStart(Vector2 start);
    Vector2 getStart();
    
    void setEnd(Vector2 end);
    Vector2 getEnd();
    
    void setRestitution(float restitution);
    float getRestitution();
    
    void addParticle(Particle* m);
    std::vector<Particle*> getParticles();
    
    unsigned checkForContact(ParticleContact *contact,unsigned limit) const;
};
