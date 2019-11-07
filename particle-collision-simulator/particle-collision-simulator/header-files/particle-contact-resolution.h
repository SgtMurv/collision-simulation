//The classes in this file are responsible for modelling the resolution of contacts in the simulation
#ifndef PCONTACTS_H
#define PCONTACTS_H

#include "particle.h"
#include <vector>

class ParticleContactResolver;

class ParticleContact{
    friend ParticleContactResolver;
public:
    // particle array of the particles involved in the contact, the second can be null (e.g. if the particle is contacting a platform)
    Particle* particle[2];
    // coefficient that determines the bouncyness of the particles after contact
    float restitution;
    // the direction of the contact
    Vector2 contactNormal;
    // how much the particles are overlapping each other
    float penetration;
protected:
    // function which resolves the contact, both in terms of velocity and penetration.
    void resolve(float duration);
    // calculates the separating velocity at this contact
    float calculateSeparatingVelocity() const;
private:
    // calculates the impulse of this collision
    void resolveVelocity(float duration);
};

// contains the routine to handle multiple particle contacts, one instance is used throughout the simulation
class ParticleContactResolver{
protected:
    // the number of collisions the simulation can resolve in one time step
    unsigned numberOfCollisionsThatCanBeResolved;
    //purely used for performance tracking -> keeps track of the actual number of iterations used
    unsigned numberofCollisionsActuallyResolved;
public:
    // creates a new contact resolver
    ParticleContactResolver(unsigned numberOfCollisionsThatCanBeResolved);
    void setNumberOfCollisionsThatCanBeResolved(unsigned numberOfCollisionsThatCanBeResolved);
    // resolves a set of particle contacts for both penetration and velocity
    void resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration);
};

#endif // CONTACTS_H
