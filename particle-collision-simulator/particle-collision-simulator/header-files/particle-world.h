#ifndef PARTICLE_WORLD_H
#define PARTICLE_WORLD_H

#include <vector>
#include "particle-contact-resolution.h"
#include "particle-contact-generator.h"
class ParticleWorld{
protected:
    std::vector<Particle*> particles;
    std::vector<ParticleContactGenerator*> particleContactGenerators;
    bool calculateNumberOfCollisionsThatCanBeResolved;
    ParticleContactResolver resolver;
    ParticleContact *contacts;
    unsigned maxContacts;
public:
    
    ParticleCollision particleCollisionGenerator;
    ParticleWorld(unsigned maxContacts, unsigned numberOfCollisionsThatCanBeResolved=0);
    ~ParticleWorld();
    unsigned generateContacts();
    void integrate(float duration);
    void runPhysics(float duration);
    std::vector<Particle*>& getParticles();
    std::vector<ParticleContactGenerator*>& getContactGenerators();
};

#endif // PARTICLE_WORLD_H
