#pragma once

#include "Ponto.h"

class Camera
{
private:
    Ponto *position;
    Ponto *lookAt;
    Ponto *up;
    float fov, near, far;

public:
    Camera() 
    : position(new Ponto(0.0f,0.0f,0.0f)), lookAt(new Ponto(0.0f,0.0f,0.0f)), up(new Ponto(0.0f,1.0f,0.0f)), fov(0.45f), near(1.0f), far(1000.0f) {};
    Camera(Ponto *position, Ponto *lookAt, Ponto *up) 
    : position(position), lookAt(lookAt), up(up), fov(0.45f), near(1.0f), far(1000.0f) {};
    Camera(Ponto *position, Ponto *lookAt, Ponto *up, float fov, float near, float far) 
    : position(position), lookAt(lookAt), up(up), fov(fov), near(near), far(far) {};
    ~Camera(){};

    Ponto *getPosition();
    Ponto *getLookAt();
    Ponto *getUp();
    float getFov();
    float getNear();
    float getFar();

    void setPosition(Ponto* position);
    void setLookAt(Ponto* lookAt);
    void setUp(Ponto* up);
    void setFov(float fov);
    void setNear(float near);
    void setFar(float far);
};