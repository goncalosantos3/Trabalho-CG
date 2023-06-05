#include "headers/Point2D.h"

Point2D Point2D::operator*(const float& num)
{
	return Point2D(x*num, y*num);
}

Point2D Point2D::operator+(const Point2D p2)
{
	return Point2D(x+p2.x, y+p2.y);
}

Point2D Point2D::operator-(const Point2D vec) // calculate vector from 2 points
{
	return Point2D(vec.x-x, vec.y-y);
}

float Point2D::getX()
{
	return this->x;
}

float Point2D::getY()
{
	return this->y;
}
