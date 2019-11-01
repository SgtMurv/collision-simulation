
#include "particle-world.h"

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned numberOfCollisionsThatCanBeResolved) :
resolver(numberOfCollisionsThatCanBeResolved),
maxContacts(maxContacts){
    contacts = new ParticleContact[maxContacts];
    calculateNumberOfCollisionsThatCanBeResolved = (numberOfCollisionsThatCanBeResolved == 0);
}

ParticleWorld::~ParticleWorld(){
    delete[] contacts;
}
unsigned ParticleWorld::generateContacts(){
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;
    
    // check for particle on particle collision
    unsigned particleCollisions = particleCollisionGenerator.checkForContact(nextContact, limit);
    limit -= particleCollisions;
    // check for particle on platform collision
    for (std::vector<ParticleContactGenerator*>::iterator generator = particleContactGenerators.begin(); generator != particleContactGenerators.end(); generator++)
    {
        unsigned used =(*generator)->checkForContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // We've run out of contacts to fill. This means we're missing
        // contacts.
        if (limit <= 0) break;
    }
    // Return the number of contacts used.
    return maxContacts - limit;
}
void ParticleWorld::integrate(float duration){
    // just iterates through the vector
    for (std::vector<Particle*>::iterator p = particles.begin(); p != particles.end(); p++)
    {
        // Remove all forces from the accumulator
        (*p)->integrate(duration);
    }
}
void ParticleWorld::runPhysics(float duration){
    integrate(duration);
    unsigned usedContacts = generateContacts();
    if (usedContacts)
    {
        if (calculateNumberOfCollisionsThatCanBeResolved) {
            resolver.setNumberOfCollisionsThatCanBeResolved(usedContacts * 2);
        }
        resolver.resolveContacts(contacts, usedContacts, duration);
    }
}
std::vector<Particle*>& ParticleWorld::getParticles(){
    return this->particles;
}
std::vector<ParticleContactGenerator*>& ParticleWorld::getContactGenerators(){
    return this->particleContactGenerators;
}
