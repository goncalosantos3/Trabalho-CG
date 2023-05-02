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
		
		void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);

		bool getAlign() {return align;};
};
