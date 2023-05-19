#include "headers/Point.h"

Point Point::operator*(const float& num)
{
	return Point(x*num, y*num, z*num);
}

Point Point::operator+(const Point& p2)
{
	return Point(x+p2.x, y+p2.y, z+p2.z);
}

Point Point::operator-(const Point& vec) // calculate vector from 2 points
{
	return Point(vec.x-x, vec.y-y, vec.z-z);
}

Point Point::operator*(const Point& vec) // cross product of vectors
{
	return Point(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

void Point::normalize()
{
	float length = sqrt(x*x + y*y + z*z);
	if (length <= 0.0000001f && length >= -0.0000001f)
	{
		x = 0;
		y = 1;
		z = 0;
	}
	else
	{
		x = x / length;
		y = y / length;
		z = z / length;
	}
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
