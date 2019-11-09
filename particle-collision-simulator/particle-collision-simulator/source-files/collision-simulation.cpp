
#include "../header-files/app.h"
#include "../header-files/collision-simulation.h"

using namespace std;

CollisionSimulation::CollisionSimulation():world(22,2){
    width = 600;
    height = 600;
    Particle *blob = new Particle;
    Particle *blob2 = new Particle;
    Particle *blob3 = new Particle;
    Particle *blob4 = new Particle;
    
    Platform *topEdge = new Platform;
    topEdge->setStart(Vector2 ( -nRange , nRange ));
    topEdge->setEnd(Vector2 ( nRange , nRange));
    topEdge->addParticle(blob);
    topEdge->addParticle(blob2);
    topEdge->addParticle(blob3);
    topEdge->addParticle(blob4);
    topEdge->setRestitution(1.0f);
    platforms.push_back(topEdge);
    world.getContactGenerators().push_back(topEdge);
    
    Platform *rightEdge = new Platform;
    rightEdge->setStart(Vector2 ( nRange , nRange ));
    rightEdge->setEnd(Vector2 ( nRange , -nRange));
    rightEdge->addParticle(blob);
    rightEdge->addParticle(blob2);
    rightEdge->addParticle(blob3);
    rightEdge->addParticle(blob4);
    rightEdge->setRestitution(1.0f);
    platforms.push_back(rightEdge);
    world.getContactGenerators().push_back(rightEdge);
    
    Platform *bottomEdge = new Platform;
    bottomEdge->setStart(Vector2 ( nRange , -nRange ));
    bottomEdge->setEnd(Vector2 ( -nRange , -nRange));
    bottomEdge->addParticle(blob);
    bottomEdge->addParticle(blob2);
    bottomEdge->addParticle(blob3);
    bottomEdge->addParticle(blob4);
    bottomEdge->setRestitution(1.0f);
    platforms.push_back(bottomEdge);
    world.getContactGenerators().push_back(bottomEdge);
    
    Platform *leftEdge = new Platform;
    leftEdge->setStart(Vector2 ( -nRange , -nRange ));
    leftEdge->setEnd(Vector2 ( -nRange , nRange));
    leftEdge->addParticle(blob);
    leftEdge->addParticle(blob2);
    leftEdge->addParticle(blob3);
    leftEdge->addParticle(blob4);
    leftEdge->setRestitution(1.0f);
    platforms.push_back(leftEdge);
    world.getContactGenerators().push_back(leftEdge);

    // Create blobs
    float commonPosition = 55.0f;
    
    blob->setPosition(0, commonPosition);
    blob->setRadius( 5 );
    blob->setVelocity(0,-150);
//    blob->setVelocity(0,0);
//    blob->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob->setAcceleration(0,0);
    blob->setMass(30.0f);
    blob->clearAccumulator();
    blob->setRed(1);
    blob->setGreen(0);
    blob->setBlue(0);
    
    blob2->setPosition(commonPosition,0);
    blob2->setRadius( 5 );
    blob2->setVelocity(-150,0);
//    blob2->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob2->setAcceleration(0,0);
    blob2->setMass(30.0f);
    blob2->clearAccumulator();
    blob2->setRed(0);
    blob2->setGreen(0);
    blob2->setBlue(1);
    
    blob3->setPosition(0, -commonPosition);
    blob3->setRadius( 5 );
    blob3->setVelocity(0,150);
//    blob3->setVelocity(0,0);
//    blob3->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob3->setAcceleration(0,0);
    blob3->setMass(30.0f);
    blob3->clearAccumulator();
    blob3->setRed(0);
    blob3->setGreen(1);
    blob3->setBlue(0);
    
    blob4->setPosition(-commonPosition, 0);
    blob4->setRadius( 5 );
    blob4->setVelocity(150,0);
//    blob4->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob4->setAcceleration(0,0);
    blob4->setMass(30.0f);
    blob4->clearAccumulator();
    blob4->setRed(1);
    blob4->setGreen(1);
    blob4->setBlue(0);

    
    particles.push_back(blob);
    particles.push_back(blob2);
    particles.push_back(blob3);
    particles.push_back(blob4);
    world.getParticles().push_back(blob);
    world.getParticles().push_back(blob2);
    world.getParticles().push_back(blob3);
    world.getParticles().push_back(blob4);
    
    //-----Quad-Tree-----
    
    //create an empty quad tree
    // QuadTree(float x, float y, float width, float height, int level, int maxLevel)
    world.root = new QuadTree(-nRange,nRange,(2*nRange),(2*nRange),0,4);
    world.particleCollisionGenerator.root = world.root;
    
    //populate the quad tree
    for(int i = 0; i< particles.size(); i++){
        world.root->addParticle(particles[i]);
    }
}

CollisionSimulation::~CollisionSimulation(){
    for(int i =0; i < particles.size();i++){
        delete particles[i];
    }
}

void CollisionSimulation::update(void){
     // Recenter the axes
    float duration = this->getTimeInterval()/1000;

    // clear the quad tree of all particles
    world.root->clearParticles();
    
    //populate the quad tree
    for(int i = 0; i< particles.size(); i++){
        world.root->addParticle(particles[i]);
    }
    
    // Run the simulation
    world.runPhysics(duration);

    Application::update();
}

void CollisionSimulation::drawLine(Vector2* start, Vector2* end){
    // logic for drawing lines in opengl
    glBegin(GL_LINES);
    glColor3f(1,0.5,0.5);
    glVertex2f(start->x, start->y);
    glVertex2f(end->x, end->y);
    glColor3f(1,0.9,0.2);
    glEnd();
}

void CollisionSimulation::display(void)
{
    Application::display();
    
//    Vector2* start = new Vector2(-50, 50);
//    Vector2* end = new Vector2(50, -50);
//
//    drawLine(start, end);
//    start = new Vector2(50, 50);
//    end = new Vector2(-50, -50);
//    drawLine(start, end);
    
//    //display all the grid areas
//    vector<Vector2*> gridAreas =  grid.getGridAreas();
//    for(int i =0; i < gridAreas.size();i++){
//        // top line
//        Vector2* start = gridAreas[i];
//        Vector2* end = new Vector2(gridAreas[i]->x + nRange, gridAreas[i]->y);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // right line
//        start = end;
//        end = new Vector2(end->x, end->y-nRange);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // bottom line
//        start = end;
//        end = new Vector2(end->x-nRange, end->y);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // left line
//        start = end;
//        end = gridAreas[i];
//        drawLine(start, end);
//
//    }
    
    //display all platforms
    for(int j = 0; j<platforms.size(); j++){
        Vector2 p0 = platforms[j]->getStart();
        Vector2 p1 = platforms[j]->getEnd();
        // logic for drawing lines in opengl
        glBegin(GL_LINES);
        glColor3f(1,0.9,0.2);
        glVertex2f(p0.x, p0.y);
        glVertex2f(p1.x, p1.y);
        glColor3f(1,0.9,0.2);
        glEnd();
    }
    
    //display all of the particles
    for (int i =0; i< particles.size(); i++){
        const Vector2 &position = particles[i]->getPosition();
        glPushMatrix();
        glTranslatef(position.x, position.y, 0.0f);
        glColor3f(particles[i]->getRed(), particles[i]->getGreen(), particles[i]->getBlue());
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
