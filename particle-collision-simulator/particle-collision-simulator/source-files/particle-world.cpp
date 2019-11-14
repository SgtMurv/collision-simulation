
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

ParticleWorld::~ParticleWorld(){
    delete[] contacts;
}
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
void ParticleWorld::integrate(float duration){
    // just iterates through the vector
    for (std::vector<Particle*>::iterator p = particles.begin(); p != particles.end(); p++)
    {
//        // work out the drag force on the particle and add the force to the forceAccum
//        // add drag force to the particle
//        float k1 = 0;
//        float k2 = 0.01*pow((*p)->getRadius(),2);
//        float magnitudeOfVelocity = (*p)->getVelocity().magnitude();
//        float fDrag = -k1 * magnitudeOfVelocity -k2 * pow(magnitudeOfVelocity,2);
//        Vector2 dragForce = (*p)->getVelocity();
//        //This line gets the sign (direction) of the current velocity so that we know what direction to apply the drag force.
//        dragForce.normalise();
//        dragForce*=fDrag;
//        (*p)->addForce(dragForce);
        
        //updates the properties of each particle based on its forceAccum
        (*p)->integrate(duration);
    }
}
void ParticleWorld::runPhysics(float duration){
    integrate(duration);
    unsigned usedContacts = generateContacts();
    // if some contacts have been noted this iteration then we need to resolve them
    
//    //check the contacts array:
//    if(contacts[0].particle[0] != nullptr){
//        for(int i=0; i< usedContacts; i++){
//            cout << "-------------------------------------" << endl;
//
//            if(contacts[i].particle[1])
//            {
//                cout << "2 Particle Collision" << endl;
//            }else{
//                cout << "Particle On Platform Collision" << endl;
//            }
//        }
//    }
    
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
