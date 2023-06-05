#pragma once
#include <iostream>
#include <math.h>

class Point2D
{
private:
	float x, y;

public:
	Point2D() :x(0.0f), y(0.0f){};
	Point2D(float p_x, float p_y) :x(p_x), y(p_y){};
	Point2D(const Point2D& p):x(p.x), y(p.y){
	};
	~Point2D() 
	{ 
	};
	
	Point2D operator*(const float& num);
	Point2D operator+(const Point2D vec); // sum point coordinates
	Point2D operator-(const Point2D vec); // calculate vector from 2 points
	
	float getX();
	float getY();
};
