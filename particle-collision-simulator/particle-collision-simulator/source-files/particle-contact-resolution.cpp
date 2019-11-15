#include <float.h>
#include "particle-contact-resolution.h"
//-------------------Particle-Contact-------------------------
// function which resolves the contact, both in terms of velocity and penetration.
void ParticleContact::resolve(float duration){
    resolveVelocity(duration);
}
// calculates the separating velocity of this contact
float ParticleContact::calculateSeparatingVelocity()const{
    Vector2 relativeVelocity = particle[0]->getVelocity();
    //checks if this contact involves 2 particles
    if (particle[1]) {
        relativeVelocity -= particle[1]->getVelocity();
    }
    return relativeVelocity * contactNormal;
}
// calculates the impulse of this collision
void ParticleContact::resolveVelocity(float duration){
    // Find the velocity in the direction of the contact
    float separatingVelocity = calculateSeparatingVelocity();

    // Check if it needs to be resolved
    if (separatingVelocity > 0)
    {
        // The contact is either separating, or stationary - there's
        // no impulse required.
        return;
    }

    // Calculate the new separating velocity
    float newSepVelocity = -separatingVelocity * restitution;
    float deltaVelocity = newSepVelocity - separatingVelocity;

    // We apply the change in velocity to each object in proportion to
    // their inverse mass (i.e. those with lower inverse mass [higher
    // actual mass] get less change in velocity)
    float totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // If all particles have infinite mass, then impulses have no effect
    if (totalInverseMass <= 0) return;

    // Calculate the impulse to apply
    float impulse = deltaVelocity / totalInverseMass;

    // Find the amount of impulse per unit of inverse mass
    Vector2 impulsePerIMass = contactNormal * impulse;

    // Apply impulses: they are applied in the direction of the contact,
    // and are proportional to the inverse mass.
    particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getInverseMass());
    if (particle[1])
    {
        // Particle 1 goes in the opposite direction
        particle[1]->setVelocity(particle[1]->getVelocity() +impulsePerIMass * -particle[1]->getInverseMass());
    }
}

//-------------------Particle-Contact-Resolver-------------------------
// creates a new contact resolver
ParticleContactResolver::ParticleContactResolver(unsigned numberOfCollisionsThatCanBeResolved): numberOfCollisionsThatCanBeResolved(numberOfCollisionsThatCanBeResolved){}

// setter for the numberOfCollisionsThatCanBeResolved.
void ParticleContactResolver::setNumberOfCollisionsThatCanBeResolved(unsigned numberOfCollisionsThatCanBeResolved){
    this->numberOfCollisionsThatCanBeResolved = numberOfCollisionsThatCanBeResolved;
}
// resolves a set of particle contacts for both penetration and velocity
void ParticleContactResolver::resolveContacts(ParticleContact *contactArray, unsigned numContacts, float duration){
    unsigned i;
    this->numberofCollisionsActuallyResolved = 0;
    // loops until all of the collisions have been resolved
    while(numberofCollisionsActuallyResolved < numberOfCollisionsThatCanBeResolved)
    {
        // Find the contact with the largest closing velocity;
        float max = DBL_MAX;
        unsigned maxIndex = numContacts;
        for (i = 0; i < numContacts; i++)
        {
            float sepVel = contactArray[i].calculateSeparatingVelocity();
            if (sepVel < max &&
                (sepVel < 0 || contactArray[i].penetration > 0))
            {
                max = sepVel;
                maxIndex = i;
            }
        }
         //Do we have anything worth resolving?
        if (maxIndex == numContacts) break;

        // Resolve this contact
        contactArray[maxIndex].resolve(duration);
        
        //increment the number of collisions resolved.
        numberofCollisionsActuallyResolved++;
    }
}
