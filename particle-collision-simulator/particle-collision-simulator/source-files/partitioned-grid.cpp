#include "../header-files/partitioned-grid.h"
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
        //check which grid area the particle is in
        for(int j=0; j<gridAreas.size();j++){
            if(posI.x > gridAreas[j]->x &&
               posI.x < gridAreas[j]->x + nRange &&
               posI.y < gridAreas[j]->y &&
               posI.y > gridAreas[j]->y - nRange){
                // the particle belongs to this quadrant
                
                //how to add an item to a data structure via an index????
                
                particleGrid[j].push_back(particles[i]);
                break;
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

//void PartitionedGrid::drawLine(Vector2* start, Vector2* end){
//    // logic for drawing lines in opengl -> same as for platforms
//}
//void PartitionedGrid::drawPartitions(float nRange){
//    for(int i =0; i < gridAreas.size();i++){
//        // top line
//        Vector2* start = gridAreas[i];
//        Vector2* end = new Vector2(gridAreas[i]->x + nRange, gridAreas[i]->y);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // right line
//        start = end;
//        end = new Vector2(gridAreas[i]->x, gridAreas[i]->y-nRange);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // bottom line
//        start = end;
//        end = new Vector2(gridAreas[i]->x-nRange, gridAreas[i]->y);
//        drawLine(start, end);
//        delete end; // free up memory you've allocated.
//        // left line
//        start = end;
//        end = gridAreas[i];
//        drawLine(start, end);
//
//    }
//}
