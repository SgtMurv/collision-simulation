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
// tester method just to print the level of each quad tree in the quad tree.
void QuadTree::printLevels(){
    cout << "Level -> "<< level << endl;
    if(level < maxLevel){
        NW->printLevels();
        NE->printLevels();
        SW->printLevels();
        SE->printLevels();
    }
}

bool QuadTree::contains(QuadTree *child, Particle *particle){
    return !(
    particle->getPosition().x < child->x ||
    particle->getPosition().y < child->y ||
    particle->getPosition().x > child->x + child->width ||
    particle->getPosition().y > child->y + child->height ||
    particle->getPosition().x - particle->getRadius() < child->x ||
    particle->getPosition().y - particle->getRadius() < child->y ||
    particle->getPosition().x + particle->getRadius() > child->x + child->width ||
    particle->getPosition().y + particle->getRadius() > child->y + child->height
    );
}

void QuadTree::addParticle(Particle* particle){
    // we dont want to make an infinite amount of partitions so if the level is max level we add the particle to this quad tree and we return to stop the recursion
    if (level == maxLevel) {
        particles.push_back(particle);
        return;
    }
    if (contains(NW, particle)) {
        NW->size++;
        NW->addParticle(particle); return;
    } else if (contains(NE, particle)) {
        NE->size++;
        NE->addParticle(particle); return;
    } else if (contains(SW, particle)) {
        SW->size++;
        SW->addParticle(particle); return;
    } else if (contains(SE, particle)) {
        SE->size++;
        SE->addParticle(particle); return;
    }
    if (contains(this, particle)) {
        particles.push_back(particle);
    }
}
