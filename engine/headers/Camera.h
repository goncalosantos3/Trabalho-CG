#pragma once

#include "Point.h"

class Camera
{
private:
    Point *position;
    Point *lookAt;
    Point *up;
    float fov, near, far;

public:
    Camera() 
    : position(new Point(0.0f,0.0f,0.0f)), lookAt(new Point(0.0f,0.0f,0.0f)), up(new Point(0.0f,1.0f,0.0f)), fov(0.45f), near(1.0f), far(1000.0f) {};
    Camera(Point *position, Point *lookAt, Point *up) 
    : position(position), lookAt(lookAt), up(up), fov(0.45f), near(1.0f), far(1000.0f) {};
    Camera(Point *position, Point *lookAt, Point *up, float fov, float near, float far) 
    : position(position), lookAt(lookAt), up(up), fov(fov), near(near), far(far) {};
    ~Camera(){};

    Point *getPosition();
    Point *getLookAt();
    Point *getUp();
    float getFov();
    float getNear();
    float getFar();

    void setPosition(Point* position);
    void setLookAt(Point* lookAt);
    void setUp(Point* up);
    void setFov(float fov);
    void setNear(float near);
    void setFar(float far);
};