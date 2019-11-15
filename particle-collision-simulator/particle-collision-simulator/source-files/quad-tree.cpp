#include "../header-files/quad-tree.h"
#include <iostream>
using namespace std;

QuadTree::QuadTree(float x, float y, float width, float height, int level, int maxLevel):x(x),y(y),width(width),height(height),level(level),maxLevel(maxLevel){ // setting the param values to the values of the object
    
    // check if the current level is the maxLevel
    if(level < maxLevel){
        // if this is not the max level of the quad tree then we need to create another level , so we instantiate the contructor of the child quad trees
        NW = new QuadTree(x             ,y              ,(width/2),(height/2),level+1,maxLevel);
        NE = new QuadTree(x+(width/2)   ,y              ,(width/2),(height/2),level+1,maxLevel);
        SW = new QuadTree(x             ,y-(height/2)   ,(width/2),(height/2),level+1,maxLevel);
        SE = new QuadTree(x+(width/2)   ,y-(height/2)   ,(width/2),(height/2),level+1,maxLevel);
    }
    else{
        // If we are at the bottom level then we dont need another level and do not instantiate the constructors of the child quad trees
        NW = nullptr;
        NE = nullptr;
        SW = nullptr;
        SE = nullptr;
    }
}
// test method to print the level & Particles for each quad tree in the quad tree. This was used during development to confirm that the correct particles were in each quadrant of the tree.
void QuadTree::printDetails(){
    cout << "Level -> "<< level << endl;
    if(particles.size() > 0){
        for(int i = 0; i< particles.size();i++){
            Vector2 pos = particles[i]->getPosition();
            cout << "--Particle["<< pos.x << ", " << pos.y <<"]" << endl;
        }
    }
    if(level < maxLevel){
        // if this is not the bottom level of the quad tree then call the method on this trees child nodes.
        NW->printDetails();
        NE->printDetails();
        SW->printDetails();
        SE->printDetails();
    }
}

// method used to add particles to the quad tree. The idea is that there are a number of checks that are done to determine what quadrant of the last layer of the tree that this particle should belong to and then it is added to that quadrant.
void QuadTree::addParticle(Particle* particle){
    // if we are at the bottom of the quad tree then we add the particle to this quad tree's vector of particles (as the particle has been passed down by another quad tree already)
    if(level == maxLevel){
        particles.push_back(particle);
        return;
    }
    Vector2 position = particle->getPosition();
    float radius = particle->getRadius();
    // check if the particle or the particle+radius (just in case the particle overlaps the vertical axis a bit)  is to the left or right of the vertical midpoint of the current quad tree.
    if(position.x < (x + (width/2)) ||
       position.x - radius < (x + (width/2))){
        // check if the particle is above or bellow the horizontal axis
        if(position.y > (y - (height/2)) ||
           position.y + radius > (y - (height/2))){
            //if its above the line, add the particle to the NW QuadTree
            NW->addParticle(particle);
        }
        if(position.y < (y - (height/2)) ||
           position.y - radius < (y - (height/2))){
            //if its bellow the line, add the particle to the SW QuadTree
            SW->addParticle(particle);
        }
    }
    if(position.x > (x + (width/2)) ||
       position.x + radius > (x + (width/2))){
        // check if the particle is above or bellow the horizontal axis
        if(position.y > (y - (height/2)) ||
           position.y + radius > (y - (height/2))){
            //if its above the line, add the particle to the NE QuadTree
            NE->addParticle(particle);
        }
        if(position.y < (y - (height/2)) ||
           position.y - radius < (y - (height/2))){
            //if its bellow the line,add the particle to the SE QuadTree
            SE->addParticle(particle);
        }
    }
}

// the purpose of this method is to populate the vector of particle vectors with the partitions that have more than 1 particles in it. This returned vector of vectors will be looped over then so that only the particles that can collide will be checked.
void QuadTree::getPotentialCollidingParticles(vector<vector<Particle*>>& potentialCollisions){
    
    if(level < maxLevel){
        //if we are not at the bottom of the quad tree, there are no particles in the vector so we need to call the same method on the child quad trees.
        NW->getPotentialCollidingParticles(potentialCollisions);
        NE->getPotentialCollidingParticles(potentialCollisions);
        SW->getPotentialCollidingParticles(potentialCollisions);
        SE->getPotentialCollidingParticles(potentialCollisions);
    }
    // this tree must be at the bottom but the code will only run if it contains at least 2 nodes.
    else if(particles.size() > 1){
        // if it does then we add it to the vector of vectors.
        potentialCollisions.push_back(particles);
    }
    // end the execution of the recursive functions.
    return;
}

// at each iteration the particles can move between the partitions so, we clear the vector of particles from each node so that we can re-populate when the particles have moved.
void QuadTree::clearParticles(){
    if(level < maxLevel){
        // if we are at the lowest level in the quad tree then we call the clearParticles method of the child nodes.
        NW->clearParticles();
        NE->clearParticles();
        SW->clearParticles();
        SE->clearParticles();
    }
    // if a quad tree at the bottom level has some particles in it then the vector is cleared.
    else if(particles.size() > 0){
        particles.clear();
    }
}

