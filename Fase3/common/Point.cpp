#include "headers/Point.h"

Point Point::operator*(const float& num)
{
	return Point(x*num, y*num, z*num);
}

Point Point::operator+(const Point& p2)
{
	return Point(x+p2.x, y+p2.y, z+p2.z);
}

float Point::getX()
{
	return this->x;
}

float Point::getY()
{
	return this->y;
}

float Point::getZ()
{
	return this->z;
}
