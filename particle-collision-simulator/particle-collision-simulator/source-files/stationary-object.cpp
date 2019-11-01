#include "stationary-object.h"
#include <iostream>

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
