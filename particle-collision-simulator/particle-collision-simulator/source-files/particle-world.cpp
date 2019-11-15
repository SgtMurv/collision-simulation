
#include "particle-world.h"
#include <iostream>
#include <string>

using namespace std;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned numberOfCollisionsThatCanBeResolved) :
resolver(numberOfCollisionsThatCanBeResolved),
maxContacts(maxContacts){
    // allocating the contacts ParticleContact pointer to a new array of memory on the heap with the size of maxContacts.
    contacts = new ParticleContact[maxContacts];
    calculateNumberOfCollisionsThatCanBeResolved = (numberOfCollisionsThatCanBeResolved == 0);
}

// deallocate the memory of the contacts array.
ParticleWorld::~ParticleWorld(){
    delete[] contacts;
}

// the purpose of this method is to detect if collisions have occured using the contact generators that are apart of the particle world. We then add up all the collisions that have occured in that iteration and return it so that we know how many collisions we need to resolve.
unsigned ParticleWorld::generateContacts(){
    unsigned limit = maxContacts;
    // creates a pointer that initially points at the area of memory that stores the first entry in the empty contacts array.
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

// loops through all the particles and calls their integrate methods so that they are re-positioned based on their forceAccum and their velocities.
void ParticleWorld::integrate(float duration){
    // just iterates through the vector
    for (std::vector<Particle*>::iterator p = particles.begin(); p != particles.end(); p++)
    {
        // work out the drag force on the particle and add the force to the forceAccum
        // add drag force to the particle
        float k1 = 0;
        float k2 = 0.01*pow((*p)->getRadius(),2);
        float magnitudeOfVelocity = (*p)->getVelocity().magnitude();
        float fDrag = -k1 * magnitudeOfVelocity -k2 * pow(magnitudeOfVelocity,2);
        Vector2 dragForce = (*p)->getVelocity();
        //This line gets the sign (direction) of the current velocity so that we know what direction to apply the drag force.
        dragForce.normalise();
        dragForce*=fDrag;
        (*p)->addForce(dragForce);
        
        //updates the properties of each particle based on its forceAccum
        (*p)->integrate(duration);
    }
}

// main entry point of the particle world where the contact generators are called and then the resolver is also called if there are any contacts to resolve.
void ParticleWorld::runPhysics(float duration){
    // update the particle positions
    integrate(duration);
    // generate the contacts
    unsigned usedContacts = generateContacts();
    
    // if there were contacts then they are resolved with the contact resolver.
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

// method used to dynamically set the max contacts based on the number of particles in the simulation.
void ParticleWorld::setMaxContacts(int maxContacts){
    delete[] contacts;
    this->maxContacts = maxContacts;
    contacts = new ParticleContact[maxContacts];
}
