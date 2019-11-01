#ifndef PARTICLE_WORLD_H
#define PARTICLE_WORLD_H

#include <vector>
#include "particle-contacts.h"
#include "stationary-object.h"
class ParticleWorld{
protected:
    std::vector<Particle*> particles;
    std::vector<StationaryObject*> stationaryObjects;
    bool calculateNumberOfCollisionsThatCanBeResolved;
    ParticleContactResolver resolver;
    ParticleContact *contacts;
    unsigned maxContacts;
public:
    ParticleWorld(unsigned maxContacts, unsigned numberOfCollisionsThatCanBeResolved=0);
    ~ParticleWorld();
    unsigned generateContacts();
    void integrate(float duration);
    void runPhysics(float duration);
    std::vector<Particle*>& getParticles();
    std::vector<StationaryObject*>& getContactGenerators();
};

#endif // PARTICLE_WORLD_H
