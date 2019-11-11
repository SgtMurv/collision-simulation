
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
    
    // The platforms in the application are used as barriers to keep the particles within the window
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
    
    float common = 20.0f;

    Platform *top = new Platform;
    top->setStart(Vector2 ( -common , common ));
    top->setEnd(Vector2 ( common , common));
    top->addParticle(blob);
    top->addParticle(blob2);
    top->addParticle(blob3);
    top->addParticle(blob4);
    top->setRestitution(1.0f);
    platforms.push_back(top);
    world.getContactGenerators().push_back(top);

    Platform *right = new Platform;
    right->setStart(Vector2 ( common , common ));
    right->setEnd(Vector2 ( common , -common));
    right->addParticle(blob);
    right->addParticle(blob2);
    right->addParticle(blob3);
    right->addParticle(blob4);
    right->setRestitution(.5f);
    platforms.push_back(right);
    world.getContactGenerators().push_back(right);

    Platform *bottom = new Platform;
    bottom->setStart(Vector2 ( common , -common ));
    bottom->setEnd(Vector2 ( -common , -common));
    bottom->addParticle(blob);
    bottom->addParticle(blob2);
    bottom->addParticle(blob3);
    bottom->addParticle(blob4);
    bottom->setRestitution(.25f);
    platforms.push_back(bottom);
    world.getContactGenerators().push_back(bottom);

    Platform *left = new Platform;
    left->setStart(Vector2 ( -common , -common ));
    left->setEnd(Vector2 ( -common , common));
    left->addParticle(blob);
    left->addParticle(blob2);
    left->addParticle(blob3);
    left->addParticle(blob4);
    left->setRestitution(0.0f);
    platforms.push_back(left);
    world.getContactGenerators().push_back(left);
    
    // Create blobs
    float commonPosition = 55.0f;
    
    //------Realistic-Physics-Examples---------
//    blob->setPosition(-commonPosition, 0);
//    blob->setRadius( 8 );
////    blob->setVelocity(150,0);
//    blob->setVelocity(0,0);
//    blob->setAcceleration(Vector2::GRAVITY * 20.0f);
////    blob->setAcceleration(0,0);
//    blob->setMass(200.0f);
//    blob->clearAccumulator();
//    blob->setRed(1);
//    blob->setGreen(1);
//    blob->setBlue(0);
//
//    blob2->setPosition(0,0);
//    blob2->setRadius( 8 );
//    blob2->setVelocity(0,0);
//    blob2->setAcceleration(Vector2::GRAVITY * 20.0f);
////    blob2->setAcceleration(0,0);
//    blob2->setMass(50.0f);
//    blob2->clearAccumulator();
//    blob2->setRed(0);
//    blob2->setGreen(0);
//    blob2->setBlue(1);
//
//    blob3->setPosition(commonPosition,0);
//    blob3->setRadius( 8 );
////    blob3->setVelocity(0,150);
//    blob3->setVelocity(0,0);
//    blob3->setAcceleration(Vector2::GRAVITY * 20.0f);
////    blob3->setAcceleration(0,0);
//    blob3->setMass(30.0f);
//    blob3->clearAccumulator();
//    blob3->setRed(0);
//    blob3->setGreen(1);
//    blob3->setBlue(1);
    //------Spatial-Partitioning-Examples------
    
    blob->setPosition(0, commonPosition);
    blob->setRadius( 5 );
    blob->setVelocity(0,-50);
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
    blob2->setVelocity(-50,0);
//    blob2->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob2->setAcceleration(0,0);
    blob2->setMass(30.0f);
    blob2->clearAccumulator();
    blob2->setRed(0);
    blob2->setGreen(0);
    blob2->setBlue(1);

    blob3->setPosition(-commonPosition, 0);
    blob3->setRadius( 5 );
    blob3->setVelocity(50,0);
//    blob3->setVelocity(0,0);
//    blob3->setAcceleration(Vector2::GRAVITY * 20.0f);
    blob3->setAcceleration(0,0);
    blob3->setMass(30.0f);
    blob3->clearAccumulator();
    blob3->setRed(0);
    blob3->setGreen(1);
    blob3->setBlue(0);

    blob4->setPosition(0, -commonPosition);
    blob4->setRadius( 5 );
    blob4->setVelocity(0,50);
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
    
    // set the particles vector in the world to all the new particles in the simulation
    world.getParticles() = particles;
    
    //create an empty quad tree
    // params-> QuadTree(float x, float y, float width, float height, int level, int maxLevel)
    world.root = new QuadTree(-nRange,nRange,(2*nRange),(2*nRange),0,0);
    world.particleCollisionGenerator.root = world.root;
    
    //populate the quad tree
    for(int i = 0; i< particles.size(); i++){
        world.root->addParticle(particles[i]);
    }
    // display the levels of all the quad trees along with any particles they contain
    world.root->printDetails();
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
    // displaying the partitions of the grid
    int numberOfColumnsAndRows = pow(2, world.root->maxLevel);
    float totalLength = 2* nRange;
    float widthOfPartition = totalLength/numberOfColumnsAndRows;
    int x = -nRange;
    int y = nRange;
    Vector2* start = new Vector2(x, y);
    Vector2* end = new Vector2(x, y);
    for(int i =0; i < numberOfColumnsAndRows;i++){
        // horizontal lines
        start = new Vector2(x,y - widthOfPartition);
        end = new Vector2(x+totalLength, y - widthOfPartition);
        drawLine(start, end);
        y = y - widthOfPartition;
    }
    x = -nRange;
    y = nRange;
    for(int i =0; i < numberOfColumnsAndRows;i++){
        // vertical lines
        start = new Vector2(x+ widthOfPartition,y);
        end = new Vector2(x+widthOfPartition, y - totalLength);
        drawLine(start, end);
        x = x + widthOfPartition;
    }

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
    Application::display();
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
