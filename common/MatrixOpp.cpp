#include "headers/MatrixOpp.h"

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


void PointMatrix_x_FloatMatrix(Point *M1, float *M2, Point *res)
{
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
		{
			res[i*4 + j] = Point(0,0,0);
			for (int k = 0 ; k<4 ; k++)
				res[i*4 + j] = res[i*4 + j] + M1[i*4 + k]*M2[j*4 + k];
		}
	}
}

void FloatMatrix_x_PointMatrix(float *M1, Point *M2, Point *res)
{
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
		{
			res[i*4 + j] = Point(0,0,0);
			for (int k = 0 ; k<4 ; k++)
				res[i*4 + j] = res[i*4 + j] + M2[j*4 + k]*M1[i*4 + k];
		}
	}
}

void multMatrixVector(float *M, float *V, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += V[k] * M[j * 4 + k];
		}
	}
}

void multPointMatrixVector(Point *M, float *V, Point *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = Point(0,0,0);
		for (int k = 0; k < 4; ++k) {
			res[j] = res[j] + M[j * 4 + k] * V[k];
		}
	}
}

void multMatrixPointVector(float *M, Point *V, Point *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = Point(0,0,0);
		for (int k = 0; k < 4; ++k) {
			res[j] = res[j] + V[k] * M[j * 4 + k];
		}
	}
}
