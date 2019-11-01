#include "particle-contact-generator.h"
#include <iostream>
#include <math.h>

using namespace std;

void Platform::setStart(Vector2 start){
    this->start = start;
}
Vector2 Platform::getStart(){
    return this->start;
}

void Platform::setEnd(Vector2 end){
    this->end = end;
}
Vector2 Platform::getEnd(){
    return this->end;
}

void Platform::setRestitution(float restitution){
    this->restitution = restitution;
}
float Platform::getRestitution(){
    return this->restitution;
}

void Platform::addParticle(Particle* m){
    this->particles.push_back(m);
}
vector<Particle*> Platform::getParticles(){
    return this->particles;
}

unsigned Platform::checkForContact(ParticleContact *contact ,unsigned limit) const{
    unsigned used = 0;
        for(int i = 0; i < particles.size();i++){
            // Check for penetration
            Vector2 toParticle = particles[i]->getPosition() - start;
            Vector2 lineDirection = end - start;
            
            float projected = toParticle * lineDirection;
            float platformSqLength = lineDirection.squareMagnitude();
            float squareRadius = particles[i]->getRadius()*particles[i]->getRadius();;
            
            if (projected <= 0){
                // The blob is nearest to the start point
                if (toParticle.squareMagnitude() < squareRadius){
                    // We have a collision
                    contact->contactNormal = toParticle.unit();
                    contact->restitution = restitution;
                    contact->particle[0] = particles[i];
                    contact->particle[1] = 0;
                    contact->penetration = particles[i]->getRadius() - toParticle.magnitude();
                    used ++;
                    contact ++;
                }
                
            }
            else if (projected >= platformSqLength)
            {
                // The blob is nearest to the end point
                toParticle = particles[i]->getPosition() - end;
                if (toParticle.squareMagnitude() < squareRadius)
                {
                    // We have a collision
                    contact->contactNormal = toParticle.unit();
                    contact->restitution = restitution;
                    contact->particle[0] = particles[i];
                    contact->particle[1] = 0;
                    contact->penetration = particles[i]->getRadius() - toParticle.magnitude();
                    used ++;
                    contact ++;
                }
            }
            else
            {
                // the blob is nearest to the middle.
                float distanceToPlatform = toParticle.squareMagnitude() - projected*projected / platformSqLength;
                if (distanceToPlatform < squareRadius)
                {
                    // We have a collision
                    Vector2 closestPoint = start + lineDirection*(projected/platformSqLength);
                    Vector2 position =particles[i]->getPosition();
                    
                    contact->contactNormal = (position - closestPoint).unit();
                    contact->restitution = restitution;
                    contact->particle[0] = particles[i];
                    contact->particle[1] = 0;
                    contact->penetration = particles[i]->getRadius() - sqrt(distanceToPlatform);
                    used ++;
                    contact ++;
                }
            }
        }
        return used;
}
//--------------Particle-On-Particle-Collision--------------


// some sort of error here as the particles collide but one particle pushes the other one along.


unsigned ParticleCollision::checkForContact(ParticleContact *contact,unsigned limit) const{
    unsigned used = 0;
    // need to check if the collision has occured between each and every particle
    for(int i =0; i< particles.size();i++){
        //setting it to i+1 so we dont check the collisions twice or between the same particle
        for(int j = i+1; j< particles.size();j++){
            //todo: get rid of the sqrt eventually as its less efficient
            Vector2 posI = particles[i]->getPosition();
            Vector2 posJ = particles[j]->getPosition();
            float distance = sqrt(pow((posI.x-posJ.x), 2)+ pow((posI.y-posJ.y), 2));
            if ((distance - particles[i]->getRadius() - particles[j]->getRadius()) <= 0){
                // a collision has occured so we need to populate a particle contact object
                contact->contactNormal = (posI - posJ).unit();
                contact->restitution = 1.0f; // not sure about restitution between particles
                contact->particle[0] = particles[i];
                contact->particle[1] = particles[j];
                contact->penetration = distance * -1;
                used ++;
            }
        }
    }
    return used;
}
