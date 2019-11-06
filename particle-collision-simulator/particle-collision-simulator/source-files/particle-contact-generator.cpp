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
//---------------Particle-On-Platform-Collision---------------------
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
            else if (projected >= platformSqLength){
                // The blob is nearest to the end point
                toParticle = particles[i]->getPosition() - end;
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
            else{
                // the blob is nearest to the middle.
                float distanceToPlatform = toParticle.squareMagnitude() - projected*projected / platformSqLength;
                if (distanceToPlatform < squareRadius){
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

//----Brute-Force-Approach-----
//unsigned ParticleCollision::checkForContact(ParticleContact *contact,unsigned limit) const{
//    unsigned used = 0;
//    unsigned bruteForceComparisons = 0;
//    // need to check if the collision has occured between each and every particle
//    for(int i =0; i< particles.size();i++){
//        // i+1 so we dont check the repeat collisions (1,2 and 2,1) or between the same particle(1,1)
//        for(int j = i+1; j< particles.size();j++){
//            //comparrison is being made so update the counter
//            bruteForceComparisons++;
//            //todo: get rid of the sqrt eventually as its less efficient
//            Vector2 posI = particles[i]->getPosition();
//            Vector2 posJ = particles[j]->getPosition();
//            float distance = sqrt(pow((posI.x-posJ.x), 2)+ pow((posI.y-posJ.y), 2));
//            if ((distance - particles[i]->getRadius() - particles[j]->getRadius()) <= 0){
//                // a collision has occured so we need to populate a particle contact object
//                contact->contactNormal = (posI - posJ).unit();
//                contact->restitution = 1.0f; // not sure about restitution between particles
//                contact->particle[0] = particles[i];
//                contact->particle[1] = particles[j];
//                contact->penetration = distance * -1;
//                used ++;
//            }
//        }
//    }
//    // output the number of comparisons done in this methdology
//    cout << "Number of Comparisons -> "<< bruteForceComparisons << endl;
//    return used;
//}
unsigned ParticleCollision::checkForContact(ParticleContact *contact,unsigned limit) const{
    unsigned used = 0;
    unsigned bruteForceComparisons = 0;
    // loop through the particle grid
    for(int i =0; i< particleGrid.size(); i++){
        //check if there are more than one particle in the grid area
        if(particleGrid[i].size() > 1){
            // need to check if the collision has occured between each and every particle
            for(int j =0; j< particleGrid[i].size();j++){
                // j+1 so we dont check the repeat collisions (1,2 and 2,1) or between the same particle(1,1)
                for(int x = j+1; x< particleGrid[i].size();x++){
                    // comparrison is being made so update the counter
                    bruteForceComparisons++;
                    //todo: get rid of the sqrt eventually as its less efficient
                    Vector2 posJ = particleGrid[i][j]->getPosition();
                    Vector2 posX = particleGrid[i][x]->getPosition();
                    float distance = sqrt(pow((posJ.x-posX.x), 2)+ pow((posX.y-posJ.y), 2));
                    if ((distance - particleGrid[i][j]->getRadius() - particleGrid[i][x]->getRadius()) <= 0){
                        // a collision has occured so we need to populate a particle contact object
                        contact->contactNormal = (posJ - posX).unit();
                        contact->restitution = 1.0f; // not sure about restitution between particles
                        contact->particle[0] = particleGrid[i][j];
                        contact->particle[1] = particleGrid[i][x];
                        contact->penetration = distance * -1;
                        used ++;
                    }
                }
            }
        }
        if (used == limit/2)break;
    }
    cout << "Number of Comparisons -> "<< bruteForceComparisons << endl;
    return used;
}
