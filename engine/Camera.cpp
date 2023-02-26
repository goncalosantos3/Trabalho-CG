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