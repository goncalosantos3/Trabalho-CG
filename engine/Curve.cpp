#include "headers/Curve.h"
#include <string.h>

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};
	float M[16], T[4] = {t*t*t, t*t, t, 1}, dT[4] = {3*t*t,2*t,1,0};

	for (int i=0 ; i<4 ; i++)
		for(int j=0 ; j<4 ; j++)
			M[i*4+j] = m[i][j];

	for (int i=0 ; i<3 ; i++) // para cada uma das componentes (x,y,z)
	{
		float A[4], P[4] = {p0[i], p1[i], p2[i], p3[i]};
		// Compute A = M * P
		multMatrixVector(M, P, A);
		// Compute pos = T * A
		pos[i] = T[0]*A[0] + T[1]*A[1] + T[2]*A[2] + T[3]*A[3];
		// compute deriv = T' * A
		deriv[i] = dT[0]*A[0] + dT[1]*A[1] + dT[2]*A[2] + dT[3]*A[3];
	}
}


// given  global t, returns the point in the curve
void Curve::getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) {
	
	int POINT_COUNT = this->curvePoints.size();
	// printf("gt %f\n", gt);
	float t = (fmod(gt, this->getTime())/this->getTime()) * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	// printf("%f\n", t);
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; 
	indices[3] = (indices[2]+1)%POINT_COUNT;

	float p[4][3];
	for (int i=0 ; i<4 ; i++)
	{
		Point pt = this->curvePoints.at(indices[i]);
		float aux[3] = {pt.getX(), pt.getY(), pt.getZ()};
		memcpy(p[i], aux, 3*sizeof(float));
	}

	getCatmullRomPoint(t, p[0], p[1], p[2], p[3], pos, deriv);
}
