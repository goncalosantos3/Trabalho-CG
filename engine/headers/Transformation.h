#pragma once

#include <vector>
#include "../../common/headers/Point.h"

using namespace std;

enum TransfType { Translate=0, TimeTranslate=1, Rotate=2, TimeRotate=3, Scale=4, Color=5 };

class Transformation
{
	private:
		enum TransfType type;
		float x, y, z;
		float angle;
		float time;

	public:
		Transformation(){};
		Transformation(enum TransfType type, float time): type(TimeTranslate), time(time){};
		Transformation(enum TransfType type, float x, float y, float z): type(type), x(x), y(y), z(z), angle(-1), time(-1){};
		Transformation(float x, float y, float z, float time): type(TimeRotate), x(x), y(y), z(z), time(time), angle(-1){};
		Transformation(enum TransfType type, float x, float y, float z, float angle): type(type), x(x), y(y), z(z), angle(angle), time(-1){};
		~Transformation(){};

		enum TransfType getType();
		float getX();
		float getY();
		float getZ();
		float getAngle();
};
