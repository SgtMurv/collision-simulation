//
//  app.cpp
//  sphere-tutorial-2
//
//  Created by Matthew Honour on 27/09/2019.
//  Copyright © 2019 Matthew Honour. All rights reserved.
//
#define GL_SILENCE_DEPRECATION // this line silences the yellow warnings of deprecation of OpenGL from Xcode.
#include <GLUT/GLUT.h>
#include "../header-files/app.h"


void Application::initGraphics(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}
void Application::display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void Application::update()
{
    // Tells glut that the display needs to be  re-rendered
    glutPostRedisplay();
}
void Application::resize(int width, int height){
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;
    // Prevent a divide by zero
    if(height == 0) height = 1;
    // Set Viewport to window dimensions
    glViewport(0, 0, width, height);
    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Establish clipping volume (left, right, bottom, top, near, far)
    if (width <= height){
        Application::width = nRange;
        Application::height = nRange/aspectRatio;
        glOrtho (-nRange, nRange, -nRange/aspectRatio, nRange/aspectRatio, -nRange*2.0f, nRange*2.0f);
    }
    else{
        Application::width = nRange*aspectRatio;
        Application::height = nRange;
        glOrtho (-nRange*aspectRatio, nRange*aspectRatio, -nRange, nRange, -nRange*2.0f, nRange*2.0f);
    }
    // Reset the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float Application::getTimeInterval(){
    return this->timeInterval;
}
void Application::setTimeInterval(float timeInterval){
    this->timeInterval = timeInterval;
}
int Application::getHeight(){
    return this->height;
}
int Application::getWidth(){
    return this->width;
}
