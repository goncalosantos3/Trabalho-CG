#pragma once

enum TransfType { Translate=0, TimeTranslate=1, Rotate=2, TimeRotate=3, Scale=4, Color=5 };

class Transformation
{
	private:
		enum TransfType type;
		float x, y, z;
		float angle;
		bool align;
		float time;

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
