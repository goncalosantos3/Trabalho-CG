#pragma once

#include "../../common/headers/Point.h"
#include <cmath>

class Camera
{
private:
    Point *position;
    Point *lookAt;
    Point *up;
    float fov, near, far;
		float radius, beta, alpha;

public:
    Camera() 
    : position(new Point(0.0f,0.0f,0.0f)), lookAt(new Point(0.0f,0.0f,0.0f)), up(new Point(0.0f,1.0f,0.0f)), fov(0.45f), near(1.0f), far(1000.0f),radius(1),beta(0),alpha(0) {};
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
		float getRadius();
		float getAlpha();
		float getBeta();

    void setPosition(Point* position);
    void setLookAt(Point* lookAt);
    void setUp(Point* up);
    void setFov(float fov);
    void setNear(float near);
    void setFar(float far);
		void setRadius(float radius);
		void setAlpha(float alpha);
		void setBeta(float beta);

		void calculateAngles();
		void updateLookAt();
};
