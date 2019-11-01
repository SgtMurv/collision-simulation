#define GL_SILENCE_DEPRECATION // this line silences the yellow warnings of deprecation of OpenGL from
#include <GLUT/GLUT.h>
#include "../header-files/app.h"
#include "../header-files/collision-simulation.h"
#include <math.h>
#include <iostream>

using namespace std;

CollisionSimulation::CollisionSimulation():world(2,1){
    width = 600;
    height = 600;
    Particle *blob = new Particle;
    
    Platform *platformOne = new Platform;
    platformOne->setStart(Vector2 ( -40.0 , 0 ));
    platformOne->setEnd(Vector2 ( 40.0 , 0));
    platformOne->addParticle(blob);
    platformOne->setRestitution(1.0f);
    platforms.push_back(platformOne);
    world.getContactGenerators().push_back(platformOne);

    // Create blob
    blob->setPosition(0.0, 10.0);
    blob->setRadius( 5 );
    blob->setVelocity(0,0);
    blob->setAcceleration(Vector2::GRAVITY * 20.0f );
    blob->setMass(30.0f);
    blob->clearAccumulator();
    
    particles.push_back(blob);
    world.getParticles().push_back(blob);
}

CollisionSimulation::~CollisionSimulation(){
    for(int i =0; i < particles.size();i++){
        delete particles[i];
    }
}

void CollisionSimulation::update(void)
{
    // Recenter the axes
    float duration = this->getTimeInterval()/1000;
    // Run the simulation
    world.runPhysics(duration);
    
    Application::update();
}
void CollisionSimulation::display(void)
{
    Application::display();
    // display all of the platforms
//    for(int j = 0; j<platforms.size(); j++){
//        glPushMatrix();
//        glBegin(GL_LINES);
//        glVertex2f(platforms[j]->getStart().x, platforms[j]->getStart().y);
//        glVertex2f(platforms[j]->getEnd().x, platforms[j]->getEnd().y);
//        glEnd();
//        glColor3f(1,0,0);
//        glPopMatrix();
//    }
    const Vector2 &p0 = platforms[0]->getStart();
    const Vector2 &p1 = platforms[0]->getEnd();

    glBegin(GL_LINES);
    glColor3f(0,1,1);
    glVertex2f(p0.x, p0.y);
    glVertex2f(p1.x, p1.y);
    glEnd();

    glColor3f(1,0,0);
    //display all of the particles
    for (int i =0; i< particles.size(); i++){
        const Vector2 &position = particles[i]->getPosition();
        glPushMatrix();
        glTranslatef(position.x, position.y, 0.0f);
//        glColor3ub(particles[i]->getRed(), particles[i]->getGreen(), particles[i]->getBlue());
        glColor3f(1, .5, 0);
        glutSolidSphere(particles[i]->getRadius(), 30, 30);
        glPopMatrix();
    }
    glutSwapBuffers();
}

void CollisionSimulation::box_collision_resolve(Particle &particle){
    float radius = particle.getRadius();
    Vector2 position = particle.getPosition();
    Vector2 velocity = particle.getVelocity();
    
    bool didHitEdge = false;
    
    //Check if the sphere is touching the edge of the window
    if(position.x > width - radius || position.x < -width + radius){
        velocity.x = -velocity.x;
        didHitEdge = true;
    }
    if(position.y > height - radius || position.y < -height + radius){
        velocity.y = -velocity.y;
        didHitEdge = true;
    }
    particle.setVelocity(velocity.x, velocity.y);
    if(didHitEdge){
        float numberOfSecondsSinceTheStart = (this->getTimeInterval() * numberOfTimeSteps)/1000.0f;
        cout << "Velocity...................= ["<< particle.getVelocity().x << ", " << particle.getVelocity().y << "]" << endl;
        cout << "Seconds since the start....= "<< numberOfSecondsSinceTheStart << " seconds" <<endl;
        cout << "X..........................= " << particle.getPosition().x << endl;
        cout << "Y..........................= " << particle.getPosition().y << endl;
        cout << "=========================================================" << endl;
    }
}

//tests to see if the particle is out of the window
bool CollisionSimulation::out_of_the_box_test(Particle particle){
    float radius = particle.getRadius();
    Vector2 position = particle.getPosition();
    
    //Check if the sphere is outside of the window and if so resets its position to the
    if((position.x > width - radius) || (position.x < -width + radius))return true;
    if((position.y > height - radius) || (position.y < -height + radius))return true;
    return false;
}

//moves the particle back into the window
void CollisionSimulation::out_of_the_box_resolve(Particle &particle){
    float radius = particle.getRadius();

    Vector2 position = particle.getPosition();
    //Check if the sphere is outside of the window and if so resets its position to the
    if(position.x > width - radius){
        position.x = width - radius;
    }
    else if (position.x < -width + radius){
        position.x = -width + radius;
    }
    if(position.y > height - radius){
        position.y = height - radius;
    }
    else if (position.y < -height + radius){
        position.y = -height + radius;
    }
    particle.setPosition(position.x, position.y);
}

//this methods is not a part of the sphere class and only exists to get the sphere demo object
Application* getApplication(){
     return new CollisionSimulation();
}


//old update method:

////increment the number of time steps done
//numberOfTimeSteps += 1;
////Loop through all of the particles and update their positions
//for (int i =0; i< particles.size(); i++){
//    float duration = this->getTimeInterval()/1000.0f;
//
//    // accumulate the acceleration due to gravity to the forceAccum
//    particles[i].addForce(Vector2(0, -g*particles[i].getMass()));
//
//    // work out the drag force on the particle and add the force to the forceAccum
//    // add drag force to the particle
//    float k1 = 0;
//    float k2 = 0.01*pow(particles[i].getRadius(),2);
//    float magnitudeOfVelocity = particles[i].getVelocity().magnitude();
//    float fDrag = -k1 * magnitudeOfVelocity -k2 * pow(magnitudeOfVelocity,2);
//    Vector2 dragForce = particles[i].getVelocity();
//    //This line gets the sign (direction) of the current velocity so that we know what direction to apply the drag force.
//    dragForce.normalise();
//    dragForce*=fDrag;
//    particles[i].addForce(dragForce);
//
//    particles[i].integrate(duration);
//
//    box_collision_resolve(particles[i]);
//    if(out_of_the_box_test(particles[i])){
//        out_of_the_box_resolve(particles[i]);
//    }
//}
//
////check if the particle has collided with any other particle
//for (int i =0; i < particles.size();i++){
//    for (int j =i+1; j < particles.size();j++){
//        Vector2 positionI = particles[i].getPosition();
//        Vector2 positionJ = particles[j].getPosition();
//        float distance = sqrt(pow((positionI.x-positionJ.x), 2)+ pow((positionI.y-positionJ.y), 2));
//        distance = distance -particles[i].getRadius() - particles[j].getRadius();
//        if(distance <= 0){
//            particles[i].setVelocity(-particles[i].getVelocity().x, -particles[i].getVelocity().y);
//            particles[j].setVelocity(-particles[j].getVelocity().x, -particles[j].getVelocity().y);
//
//            // Move the particles out by half of the distance each after the collision to avoid getting trapped and contsnatly overlapping.
//            particles[i].setPosition(positionI.x + (distance/2), positionI.y + (distance/2));
//            particles[j].setPosition(positionJ.x - (distance/2), positionJ.y - (distance/2));
//        }
//    }
//}
