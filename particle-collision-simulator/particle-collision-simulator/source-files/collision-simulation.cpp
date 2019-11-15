
#include "../header-files/app.h"
#include "../header-files/collision-simulation.h"

using namespace std;

CollisionSimulation::CollisionSimulation():world(500, 500){
    width = 600;
    height = 600;
    
    // create an n number of particles
    const int particleCount = 20;
    float radius = 20;
    // setting the position of the first particle
    Vector2 position = Vector2(-nRange+radius, nRange-radius);
    const float decrease =3*radius;
    float xIncrease = decrease;
    for(int i = 0; i<particleCount; i++){
        position = Vector2(position.x+xIncrease,position.y);
        // check if the position is out the window
        if(position.x >= (nRange -radius) || position.x-radius >= (nRange -radius) ||
           position.x <= (-nRange+radius) || position.x+radius <= (-nRange+radius)){
            //if it is then alter the position to be beneath the last particle of the row and change the sign of the xIncrease so that particles are added in the other direction.
            position.y -= decrease;
            position.x -= xIncrease;
            xIncrease = -xIncrease;
        }
        //pass in the dynamic parameters for the createParticle method
        createParticle(position, radius, Vector2(10,-400), Vector2(Vector2::GRAVITY * 20.0f), 30.0f, 1, 0, 1);
    }

    // These platforms in the application are used as barriers to keep the particles within the window. They are created using the createPlaftorm method for code reuse purposes
    createPlatform(Vector2 ( -nRange , nRange ), Vector2 ( nRange , nRange), 1.0f);
    createPlatform(Vector2 ( nRange , nRange ), Vector2 ( nRange , -nRange), 1.0f);
    createPlatform(Vector2 ( nRange , -nRange ), Vector2 ( -nRange , -nRange), 1.0f);
    createPlatform(Vector2 ( -nRange , -nRange ), Vector2 ( -nRange , nRange), 1.0f);

    //initialise the particle world now that you have a number of particles and platforms
    int maxContacts = pow(particles.size(),2) * platforms.size();
    world.setMaxContacts(maxContacts);
//    world = ParticleWorld(maxContacts,particles.size());
    
    // set the particles vector in the world to all the new particles in the simulation, this is used for calling the integrate methods of all the particles in the simulation.
    world.getParticles() = particles;
    
    //create an empty quad tree, with the specified depth
    // params-> QuadTree(float x, float y, float width, float height, int level, int maxLevel)
    world.root = new QuadTree(-nRange,nRange,(2*nRange),(2*nRange),0,0);
    // the particleCollisionGenerator also need to have a reference to the quad tree so that it can check for interparticle collisions.
    world.particleCollisionGenerator.root = world.root;
    
    //populate the quad tree
    for(int i = 0; i< particles.size(); i++){
        world.root->addParticle(particles[i]);
    }
}

// method for creating platforms based on the parameters passes into it.
void CollisionSimulation::createPlatform(Vector2 start, Vector2 end, float restitution){
    Platform *newPlatform = new Platform;
    newPlatform->setStart(start);
    newPlatform->setEnd(end);
    for(int i =0; i<this->particles.size();i++){
        // platforms need a reference to all the particles so that they can check if they have collided with the platform.
        newPlatform->addParticle(this->particles[i]);
    }
    newPlatform->setRestitution(restitution);
    // the collision simulation needs a reference to the platforms so that they can be rendered into the window.
    platforms.push_back(newPlatform);
    // the platform is a contact generator and is needed in the particle world's generateContacts() method.
    world.getContactGenerators().push_back(newPlatform);
}

// method used to create particles based on the parameters passed into it
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
    
    // adding the particle to the vector so that the collision simulation can render it to the window
    this->particles.push_back(newParticle);
}

// destructor that cleans up all the memory allocated for particles and platforms
CollisionSimulation::~CollisionSimulation(){
    for(int i =0; i < particles.size();i++){
        delete particles[i];
    }
    for(int j =0; j< platforms.size();j++){
        delete platforms[j];
    }
}

//run each time the window is updated (determined by the time interval)
void CollisionSimulation::update(void){
    // make the duration a more believable value for the physics simulations
    float duration = this->getTimeInterval()/1000;

    // clear the quad tree of all particles
    world.root->clearParticles();
    
    // re-populate the quad tree, as theuy could have moved at the end of the last iteration
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

//this methods is not a part of the collision simulation class and only exists to get a new collision simulation object.
Application* getApplication(){
     return new CollisionSimulation();
}
