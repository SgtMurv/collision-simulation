//
//  particle.cpp
//  tutorial-3
//
//  Created by Matthew Honour on 04/10/2019.
//  Copyright © 2019 Matthew Honour. All rights reserved.
//

#include "particle.h"
#include "coreMath.h"
#include "assert.h"
#include <float.h>

// calculates the next position of the particle.
// Duration denotes the time interval since the last update.
void Particle::integrate(float duration){
    // No particle can have 0 mass
    if (inverseMass <= 0.0f) return;
    assert(duration > 0.0);
    
    // using the formula acc = force/mass (but as we are passing the inverse mass it counts as divide)
    Vector2 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);
    
    // alter the velocity using the resulting acceleration
    velocity.addScaledVector(resultingAcc, duration);
    
    // update the position of the particle based on its velocity
    position.addScaledVector(velocity, duration);
    
    // clear the forceAccum for the next iteration of the simulation
    clearAccumulator();
}

//----------Position----------------
void Particle::setPosition(const float x, const float y){
    this->position.x = x;
    this->position.y = y;
}
Vector2 Particle::getPosition() const{
    return this->position;
}

//----------Velocity----------------
void Particle::setVelocity(const float x, const float y){
    this->velocity.x = x;
    this->velocity.y = y;
}
Vector2 Particle::getVelocity() const{
    return this->velocity;
}

//----------Acceleration----------------
void Particle::setAcceleration(const float x, const float y){
    this->acceleration.x = x;
    this->acceleration.y = y;
}
void Particle::setAcceleration(Vector2 acceleration){
    this->acceleration = acceleration;
}
Vector2 Particle::getAcceleration()const{
    return this->acceleration;
}

//----------Mass----------------
void Particle::setMass(const float mass){
    assert(mass != 0);
    Particle::inverseMass = ((float)1.0)/mass;
}
float Particle::getMass() const{
    if (inverseMass == 0) {
        return DBL_MAX;
    } else {
        return ((float)1.0)/inverseMass;
    }
}
void Particle::setInverseMass(const float inverseMass){
    Particle::inverseMass = inverseMass;
}
float Particle::getInverseMass() const{
    return inverseMass;
}
bool Particle::hasFiniteMass() const{
    return inverseMass >= 0.0f;
}

//----------Force-Accumulator----------------
void Particle::clearAccumulator(){
    forceAccum.clear();
}
void Particle::addForce(const Vector2 &force){
    forceAccum += force;
}


//----------Radius----------------
void Particle::setRadius(const float r){
    this->radius = r;
}
float Particle::getRadius() const{
    return this->radius;
}

//----------Colors----------------
void Particle::setRed(int red){
    this->red = red;
}
int Particle::getRed()const{
    return this->red;
}
void Particle::setGreen(int green){
    this->green = green;
}
int Particle::getGreen() const{
    return this->green;
}
void Particle::setBlue(int blue){
    this->blue = blue;
}
int Particle::getBlue()const{
    return this->blue;
}

