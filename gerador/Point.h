#pragma once

class Point
{
private:
	float x, y, z;

public:
	Point() :x(0.0f), y(0.0f), z(0.0f) {};
	Point(float p_x, float p_y, float p_z) :x(p_x), y(p_y), z(p_z) {};
	~Point() {};

	float getX();
	float getY();
	float getZ();
};