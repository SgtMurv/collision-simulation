//
//  main.cpp
//  sphere-tutorial-2
//
//  Created by Matthew Honour on 27/09/2019.
//  Copyright © 2019 Matthew Honour. All rights reserved.
//
#define GL_SILENCE_DEPRECATION // this line silences the yellow warnings of deprecation of OpenGL from
#include <GLUT/GLUT.h>
#include "../header-files/app.h"

extern Application* getApplication();
Application* app;

void display(void){
    app->display();
}
void createWindow(const char* title, int h, int w){
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(w, h);
    glutCreateWindow(title);
}
void resize(int width, int height){
    app->resize(width, height);
}
void TimerFunc(int value)
{
    app->update();
    glutTimerFunc(app->getTimeInterval(), TimerFunc, 1);
}
int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    app = getApplication();
    createWindow("Sphere", app->getHeight(), app->getWidth());
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutTimerFunc(app->getTimeInterval(), TimerFunc, 1);
    app->initGraphics();
    glutMainLoop();
    delete app;
    return 0;
}
