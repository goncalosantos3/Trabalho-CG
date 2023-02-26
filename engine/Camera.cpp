#include "headers/Camera.h"

Ponto* Camera::getPosition()
{
    return this->position;
}

Ponto* Camera::getLookAt()
{
    return this->lookAt;
}

Ponto* Camera::getUp()
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

void Camera::setPosition(Ponto* position)
{
    this->position = position;
}

void Camera::setLookAt(Ponto* lookAt)
{
    this->lookAt = lookAt;
}

void Camera::setUp(Ponto* up)
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