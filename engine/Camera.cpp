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

float Camera::getRadius()
{
	return radius;
}

float Camera::getAlpha()
{
	return alpha;
}

float Camera::getBeta()
{
	return beta;
}

void Camera::setPosition(Point* position)
{
	delete this->position;
	this->position = position;
}

void Camera::setLookAt(Point* lookAt)
{
	delete this->lookAt;
	this->lookAt = lookAt;
}

void Camera::setUp(Point* up)
{
	delete this->up;
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

void Camera::setRadius(float radius)
{
	this->radius = radius;
}

void Camera::setAlpha(float alpha)
{
	this->alpha = alpha;
}

void Camera::setBeta(float beta)
{
	this->beta = beta;
}

Point* pointOnTheSameLine(Point *p1, Point *p2)
{
	float vx = p2->getX() - p1->getX(),
				vy = p2->getY() - p1->getY(),
				vz = p2->getZ() - p1->getZ();
	float len = sqrtf(vx*vx + vy*vy + vz*vz) / 10;

	return new Point(p1->getX() + vx/len, p1->getY() + vy/len, p1->getZ() + vz/len);
}

void Camera::calculateAngles()
{
    radius = sqrt(powf(position->getX() - lookAt->getX(),2) +
                  powf(position->getY() - lookAt->getY(),2) +
                  powf(position->getZ() - lookAt->getZ(),2));

    float x = position->getX(), y = position->getY(), z = position->getZ();
    float iterations = 3600,
          angle = M_PI * 2 / iterations, 
          closestY = 0.0f,
          closestXZ = 0.0f,
          closestDeltaY = abs(y - radius*cos(closestY) - lookAt->getY()),
          closestDeltaXZ =abs(abs(x + radius*sin(closestXZ) - lookAt->getX()) + abs(z + radius*cos(closestXZ) - lookAt->getZ()));
    for (int i=1 ; i<iterations ; i++)
    {
        float aux = angle*i, 
              auxDeltaY = abs(y - radius * cos(aux) - lookAt->getY()), 
              auxDeltaXZ = abs(abs(x + radius*sin(aux) - lookAt->getX()) + abs(z +  radius*cos(aux) - lookAt->getZ()));
        if (aux > -M_PI && aux < M_PI && auxDeltaY < closestDeltaY)
        {
            closestDeltaY = auxDeltaY;
            closestY = aux;
        }
        if (auxDeltaXZ < closestDeltaXZ)
        {
            closestDeltaXZ = auxDeltaXZ;
            closestXZ = aux;
        }
    }
    alpha = closestXZ;
    beta = closestY;
    updateLookAt();
}

void Camera::updateLookAt()
{
	delete lookAt;
	lookAt = new Point(position->getX() + radius*sin(alpha),
                        position->getY() - radius*cos(beta),
                        position->getZ() + radius*cos(alpha));
}
