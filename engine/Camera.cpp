#include "headers/Camera.h"

Point* Camera::getPosition()
{
    return this->position;
}

Point* Camera::getLookAt()
{
    return this->lookAt;
}

Point* Camera::getUp()
{
    return this->up;
}

float Camera::getFov()
{
    return this->fov;
}

float Camera::getNear()
{
    return this->near;
}

float Camera::getFar()
{
    return this->far;
}

void Camera::setPosition(Point* position)
{
    this->position = position;
}

void Camera::setLookAt(Point* lookAt)
{
    this->lookAt = lookAt;
}

void Camera::setUp(Point* up)
{
    this->up = up;
}

void Camera::setFov(float fov)
{
    this->fov = fov;
}

void Camera::setNear(float near)
{
    this->near = near;
}

void Camera::setFar(float far)
{
    this->far = far;
}