#pragma once
#include <iostream>
#include <math.h>

class Point
{
private:
	float x, y, z;

public:
	Point() :x(0.0f), y(0.0f), z(0.0f) {};
	Point(float p_x, float p_y, float p_z) :x(p_x), y(p_y), z(p_z) {};
	Point(const Point& p):x(p.x), y(p.y), z(p.z){
	};
	~Point() 
	{ 
	};
	
	Point operator*(const float& num);
	Point operator+(const Point& vec); // sum point coordinates
	Point operator-(const Point& vec); // calculate vector from 2 points
	Point operator*(const Point& vec); // cross product of vectors
	
	void normalize();
	void normalizeNormals();
	
	float getX();
	float getY();
	float getZ();

    float *asFloatArray();
};
