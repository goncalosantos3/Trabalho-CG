#include "headers/Transformation.h"


TransfType Transformation::getType()
{
	return this->type;
}

float Transformation::getX()
{
	return this->x;
}

float Transformation::getY()
{
	return this->y;
}

float Transformation::getZ()
{
	return this->z;
}

float Transformation::getAngle()
{
	return this->angle;
}

float Transformation::getTime()
{
	return time;
}
