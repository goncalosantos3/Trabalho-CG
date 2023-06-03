#pragma once

#include <math.h>
#include "Transformation.h"
#include "../../common/headers/MatrixOpp.h"

class Curve : public Transformation
{
	private:
		vector<Point> curvePoints;
		bool align;
	
	public:
		using Transformation::Transformation;
		Curve(float time, vector<Point> points): Transformation(TimeTranslate, time), align(false), curvePoints(points){};
		Curve(float time, bool align, vector<Point> points): Transformation(TimeTranslate, time), align(align), curvePoints(points){};
        ~Curve(){curvePoints.~vector<Point>();}
		
		void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);

		bool getAlign() {return align;};

        using Transformation::getX;
        using Transformation::getY;
        using Transformation::getZ;
        using Transformation::getTime;
        using Transformation::getAngle;
        using Transformation::getType;
        using Transformation::operator=;
};
