
#include "../header-files/app.h"
#include "../header-files/collision-simulation.h"

using namespace std;

CollisionSimulation::CollisionSimulation():world(120000,120000){
    width = 600;
    height = 600;
    
    // create particles
    const int particleCount = 20;
    float radius = 5;
    Vector2 position = Vector2(-nRange+radius, nRange-radius);
    const float decrease =3*radius;
    float xIncrease = decrease;
    for(int i = 0; i<particleCount; i++){
        position = Vector2(position.x+xIncrease,position.y);
        // check if the position is out the window
        if(position.x >= (nRange -radius) || position.x-radius >= (nRange -radius) ||
           position.x <= (-nRange+radius) || position.x+radius <= (-nRange+radius)){
            //if so then reduce the y value and change the incrementors sign
            position.y -= decrease;
            position.x -= xIncrease;
            xIncrease = -xIncrease;
        }
        createParticle(position, radius, Vector2(80,0), Vector2(Vector2::GRAVITY * 20.0f), 30.0f, 1, 0, 1);
    }
        
    // The platforms in the application are used as barriers to keep the particles within the window
    createPlatform(Vector2 ( -nRange , nRange ), Vector2 ( nRange , nRange), 1.0f);
    createPlatform(Vector2 ( nRange , nRange ), Vector2 ( nRange , -nRange), 1.0f);
    createPlatform(Vector2 ( nRange , -nRange ), Vector2 ( -nRange , -nRange), 1.0f);
    createPlatform(Vector2 ( -nRange , -nRange ), Vector2 ( -nRange , nRange), 1.0f);

    // set the particles vector in the world to all the new particles in the simulation
    world.getParticles() = particles;
    
    //create an empty quad tree
    // params-> QuadTree(float x, float y, float width, float height, int level, int maxLevel)
    world.root = new QuadTree(-nRange,nRange,(2*nRange),(2*nRange),0,3);
    world.particleCollisionGenerator.root = world.root;
    
    //populate the quad tree
    for(int i = 0; i< particles.size(); i++){
        world.root->addParticle(particles[i]);
    }
    // display the levels of all the quad trees along with any particles they contain
    world.root->printDetails();
}

void CollisionSimulation::createPlatform(Vector2 start, Vector2 end, float restitution){
    Platform *newPlatform = new Platform;
    newPlatform->setStart(start);
    newPlatform->setEnd(end);
    for(int i =0; i<this->particles.size();i++){
        newPlatform->addParticle(this->particles[i]);
    }
    newPlatform->setRestitution(restitution);
    platforms.push_back(newPlatform);
    world.getContactGenerators().push_back(newPlatform);
}

void CollisionSimulation::createParticle(Vector2 position, float radius, Vector2 velocity, Vector2 acceleration, float mass, float red, float blue, float green){
    Particle* newParticle = new Particle;
    newParticle->setPosition(position);
    newParticle->setRadius(radius);
    newParticle->setVelocity(velocity);
    newParticle->setAcceleration(acceleration);
    newParticle->setMass(mass);
    newParticle->clearAccumulator();
    newParticle->setRed(red);
    newParticle->setBlue(blue);
    newParticle->setGreen(green);
    
    this->particles.push_back(newParticle);
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
