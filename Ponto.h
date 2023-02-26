#pragma once

class Ponto
{
private:
	float x, y, z;

public:
	Ponto() :x(0.0f), y(0.0f), z(0.0f) {};
	Ponto(float p_x, float p_y, float p_z) :x(p_x), y(p_y), z(p_z) {};
	~Ponto() {};

	float getX();
	float getY();
	float getZ();
};