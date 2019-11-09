#include "../header-files/partitioned-grid.h"
#include <iostream>
using namespace std;

void PartitionedGrid::generateGridArray(vector<Particle*> particles, float nRange){
    // create the segments of the grid || Possibly make line Vector 2's, like a platform and then have a data structure that the blob demo has to loop through and display.
    Vector2 *gridArea1 = new Vector2(-nRange,nRange);
    Vector2 *gridArea2 = new Vector2(0.0f, nRange);
    Vector2 *gridArea3 = new Vector2(-nRange, 0.0f);
    Vector2 *gridArea4 = new Vector2(0.0f,0.0f);
    
    gridAreas.push_back(gridArea1);
    gridAreas.push_back(gridArea2);
    gridAreas.push_back(gridArea3);
    gridAreas.push_back(gridArea4);
    
    particleGrid.resize(gridAreas.size());
    
    // loop through all of the particles and add the particles who's x and y values are within each grid segment.
    for(int i = 0; i< particles.size();i++){
        Vector2 posI = particles[i]->getPosition();
        float radius = particles[i]->getRadius();
        //check which grid area the particle is in
        
        //TODO: Have a look at a KD apprach to it (if its left or right of x axis and then if its above or bellow y axis its like 4 * less checks)
        
        for(int j=0; j<gridAreas.size();j++){
            if((posI.x > gridAreas[j]->x || posI.x + radius > gridAreas[j]->x) &&
               (posI.y < gridAreas[j]->y || posI.y - radius < gridAreas[j]->y) &&
               (posI.x < gridAreas[j]->x + nRange || posI.x - radius < gridAreas[j]->x + nRange)  &&
               (posI.y > gridAreas[j]->y - nRange || posI.y + radius > gridAreas[j]->y - nRange)){
                // the position of the particle is in this quadrant
                particleGrid[j].push_back(particles[i]);
            }
        }
    }
}

std::vector<std::vector<Particle*>> PartitionedGrid::getParticleGrid(){
    return particleGrid;
}

void PartitionedGrid::clearDataStructures(){
    gridAreas.clear();
    particleGrid.clear();
}
std::vector<Vector2*> PartitionedGrid::getGridAreas(){
    return this->gridAreas;
}

//------Quad-Tree------
QuadTree::QuadTree(float x, float y, float width, float height, int level, int maxLevel):x(x),y(y),width(width),height(height),level(level),maxLevel(maxLevel){ // setting the param values to the values of the object
    
    // check if the current level is the maxLevel
    if(level < maxLevel){
        // instantiate the contructor of the child quad trees
        NW = new QuadTree(x             ,y              ,(width/2),(height/2),level+1,maxLevel);
        NE = new QuadTree(x+(width/2)   ,y              ,(width/2),(height/2),level+1,maxLevel);
        SW = new QuadTree(x             ,y-(height/2)   ,(width/2),(height/2),level+1,maxLevel);
        SE = new QuadTree(x+(width/2)   ,y-(height/2)   ,(width/2),(height/2),level+1,maxLevel);
    }
    else{
        // if it is then we do not instantiate the child quad trees
        NW = nullptr;
        NE = nullptr;
        SW = nullptr;
        SE = nullptr;
    }
}
// test method to print the level & Particles for each quad tree in the quad tree.
void QuadTree::printDetails(){
    cout << "Level -> "<< level << endl;
    if(particles.size() > 0){
        for(int i = 0; i< particles.size();i++){
            Vector2 pos = particles[i]->getPosition();
            cout << "--Particle["<< pos.x << ", " << pos.y <<"]" << endl;
        }
    }
    if(level < maxLevel){
        NW->printDetails();
        NE->printDetails();
        SW->printDetails();
        SE->printDetails();
    }
}



void QuadTree::addParticle(Particle* particle){
    // if we are at the bottom of the quad tree then we add the particle to our particle to this quad tree's vector of particles (as it has been passed this particle from another quad tree already)
    if(level == maxLevel){
        particles.push_back(particle);
        return;
    }
    Vector2 position = particle->getPosition();
    float radius = particle->getRadius();
    // check if the particle or the particle+radius (just in case the particle overlaps the vertical axis a bit)  is to the left or right of the vertical midpoint:
    if(position.x < (x + (width/2)) ||
       position.x - radius < (x + (width/2))){
        // check if the particle is above or bellow the horizontal axis
        if(position.y > (y - (height/2)) ||
           position.y + radius > (y - (height/2))){
            //add the particle to the NW QuadTree
            NW->addParticle(particle);
        }
        if(position.y < (y - (height/2)) ||
           position.y - radius < (y - (height/2))){
            //add the particle to the SW QuadTree
            SW->addParticle(particle);
        }
    }
    if(position.x > (x + (width/2)) ||
       position.x + radius > (x + (width/2))){
        // check if the particle is above or bellow the horizontal axis
        if(position.y > (y - (height/2)) ||
           position.y + radius > (y - (height/2))){
            //add the particle to the NE QuadTree
            NE->addParticle(particle);
        }
        if(position.y < (y - (height/2)) ||
           position.y - radius < (y - (height/2))){
            //add the particle to the SE QuadTree
            SE->addParticle(particle);
        }
    }
}

void QuadTree::getPotentialCollidingParticles(vector<vector<Particle*>>& potentialCollisions){
    
    if(level < maxLevel){
        NW->getPotentialCollidingParticles(potentialCollisions);
        NE->getPotentialCollidingParticles(potentialCollisions);
        SW->getPotentialCollidingParticles(potentialCollisions);
        SE->getPotentialCollidingParticles(potentialCollisions);
    }
    // this tree must be at the bottom and must contain at least 2 nodes so add it to the list.
    else if(particles.size() > 1){
        potentialCollisions.push_back(particles);
    }
    return;
}
void QuadTree::clearParticles(){
    if(level < maxLevel){
        NW->clearParticles();
        NE->clearParticles();
        SW->clearParticles();
        SE->clearParticles();
    }
    else if(particles.size() > 0){
        particles.clear();
    }
}

