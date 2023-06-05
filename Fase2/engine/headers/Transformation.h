#pragma once

enum TransfType { Translate=0, Rotate=1, Scale=2, Color=3 };

class Transformation
{
	private:
		enum TransfType type;
		float x, y, z;
		float angle;

	public:
		Transformation(){};
		Transformation(enum TransfType type, float x, float y, float z): type(type), x(x), y(y), z(z), angle(-1){};
		Transformation(enum TransfType type, float x, float y, float z, float angle): type(type), x(x), y(y), z(z), angle(angle){};
		~Transformation(){};

		enum TransfType getType();
		float getX();
		float getY();
		float getZ();
		float getAngle();
};
