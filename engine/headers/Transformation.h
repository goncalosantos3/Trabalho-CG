#pragma once

typedef enum { Translate=0, Rotate=1, Scale=2 } TransfType;

class Transformation
{
	private:
		TransfType type;
		float x, y, z;

	public:
		Transformation(){};
		Transformation(TransfType type, float x, float y, float z): type(type), x(x), y(y), z(z){};
		~Transformation(){};

		TransfType getType();
		float getX();
		float getY();
		float getZ();
};
