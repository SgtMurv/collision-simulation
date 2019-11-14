#pragma once
#define GL_SILENCE_DEPRECATION // this line silences the yellow warnings of deprecation of OpenGL from
#include <GLUT/GLUT.h>
#include "coreMath.h"

class Particle {
private:
    //relevant in integrate method:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float inverseMass;
    Vector2 forceAccum;
    //irrelevant to integrate method:
    GLfloat radius;
    int red;
    int green;
    int blue;
public:
    // calculates the next position of the particle.
    // Duration denotes the time interval
    //since the last update.
    virtual void integrate(float duration);
    
    void setPosition(const float x, const float y);
    void setPosition(Vector2 position);
    Vector2 getPosition() const;
    
    void setVelocity(const float x, const float y);
    void setVelocity(Vector2 velocity);
    Vector2 getVelocity() const;
    
    void setAcceleration(const float x, const float y);
    void setAcceleration(Vector2 acceleration);
    Vector2 getAcceleration() const;
        
    void setMass(const float mass);
    float getMass() const;
    void setInverseMass(const float inverseMass);
    float getInverseMass() const;
    bool hasFiniteMass() const;
    
    void clearAccumulator();
    void addForce(const Vector2 &force);
    Vector2 getForceAccum();
    
    void setRadius(const float r);
    float getRadius() const;
    
    void setRed(int red);
    int getRed()const;
    void setGreen(int green);
    int getGreen() const;
    void setBlue(int blue);
    int getBlue() const;
};
