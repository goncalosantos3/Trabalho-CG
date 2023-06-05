#pragma once

#include <math.h>
#include "Point.h"

void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void PointMatrix_x_FloatMatrix(Point *M1, float *M2, Point *res);
void FloatMatrix_x_PointMatrix(float *M1, Point *M2, Point *res);
void multMatrixVector(float *M, float *V, float *res);
void multVectorPointMatrix(Point *M, float *V, Point *res);
void multPointMatrixVector(Point *M, float *V, Point *res);
void multMatrixPointVector(float *M, Point *V, Point *res);
