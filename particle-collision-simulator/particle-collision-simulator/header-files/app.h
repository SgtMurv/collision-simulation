//
//  app.h
//  sphere-tutorial-2
//
//  Created by Matthew Honour on 27/09/2019.
//  Copyright © 2019 Matthew Honour. All rights reserved.
//

class Application{
protected:  //only inheriting classes have access to these properties.
    float nRange = 300.0f;
    int height;
    int width;
    float timeInterval = 10.0f;
//    float timeInterval = 25.0f;
    float numberOfTimeSteps = 0.0f;
public:
    virtual void initGraphics();
    virtual void display();
    virtual void update();
    virtual void resize(int width, int height);
    float getTimeInterval();
    void setTimeInterval(float timeInterval);
    int getHeight();
    int getWidth();
};

