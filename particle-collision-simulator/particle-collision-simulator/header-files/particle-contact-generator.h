#pragma once
#include "particle.h"
#include "quad-tree.h"
#include "particle-contact-resolution.h"
#include "coreMath.h"

#include <vector>
#include <list>

// interface class for stationary objects in the simulation
class ParticleContactGenerator{
public:
    virtual unsigned checkForContact(ParticleContact* &contact, unsigned limit) const = 0;
};

class Platform : public ParticleContactGenerator{
private:
    Vector2 start;
    Vector2 end;
    float restitution;
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
    
    unsigned checkForContact(ParticleContact* &contact,unsigned limit) const;
};

class ParticleCollision : public ParticleContactGenerator{
public:
    QuadTree* root;
    unsigned checkForContact(ParticleContact* &contact,unsigned limit) const;
    static void drawLineBetweenParticles(Vector2 p1, Vector2 p2);
};
