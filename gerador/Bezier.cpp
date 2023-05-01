#include "headers/Bezier.h"

using namespace std;

int numPatches, numControlPoints;
vector<vector<int>> indices;
vector<Point> controlPoints;

int divs = 16;

void readPatchFile(char *filePath)
{
	string path = "../files/";
	path.append(filePath);
	ifstream patchFile(path);

	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
	{
		cerr << "Error opening file \"" << filePath << "\".\nMake sure the file exists...";
		exit(1);
	}

	string line;

	// read number of patches
	getline(patchFile, line);
	if (sscanf(line.c_str(), "%d", &numPatches) != 1)
	{
		cerr << "ERROR: Couldn't read number of patches!! Parsing Stopped" << endl;
		exit(2);
	}

	// read indices
	indices.reserve(numPatches);

	for (int i=0 ; i<numPatches ; i++)
	{
		indices.push_back(vector<int>());
		indices.at(i).reserve(16);
		getline(patchFile, line);
		char* str = strdup(line.c_str());
		char *temp;
		while ((temp = strsep(&str, ", ")) != NULL)
		{
			if (strlen(temp) == 0)
				continue;
			int val;
			if (sscanf(temp, "%d", &val) != 1)
			{
				cerr << "ERROR: Couldn't read index!!\nParsing Stopped" << endl;
				exit(2);
			}
			indices.at(i).push_back(val);
		}
		if (indices.at(i).size() != 16)
		{
			cerr << "ERROR: Indices for path number " << i+1 << " should have 16 values!!\nParsing Stopped" << endl;
			exit(2);
		}
		free(str);
	}

	// read number of control points
	getline(patchFile, line);
	if (sscanf(line.c_str(), "%d", &numControlPoints) != 1)
	{
		cerr << "ERROR: Couldn't read number of control points!!\nParsing Stopped" << endl;
		exit(2);
	}
	
	// read control points
	controlPoints.reserve(numControlPoints);
	for (int i=0 ; i<numControlPoints ; i++)
	{
		getline(patchFile, line);
		float x,y,z;
		if (sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z) != 3)
		{
			cerr << "ERROR: Error reading Control Point number " << i << "!!\nParsing Stopped" << endl;
			exit(2);
		}
		controlPoints.emplace_back(Point(x,y,z));
	}
}

void multMatrices(float *M1, float *M2, float *res)
{
	for (int i=0 ; i<4 ; i++)
	{
		for (int j=0 ; j<4 ; j++)
		{
			res[i*4 + j] = M1[i*4 + j] * M2[j*4 + i];
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

Point getBezierPatchPoint (float u, float v, Point P[16])
{
	float M[16] ={ -1,  3, -3, 1, 
															 3, -6,  3, 0,
															-3,  3,  0, 0,
															 1,  0,  0, 0 };
	float u2 = u*u, u3 = u2*u, v2 = v*v, v3 = v2*v;
	float U[4] = {u3, u2, u, 1},
				V[4] = {v3, v2, v, 1};

	float UxM[4];
	multMatrixVector(M, U, UxM);
	Point UxMxP[4];
	multPointMatrixVector(P, UxM, UxMxP);
	Point UxMxPxMT[4];
	multMatrixPointVector(M, UxMxP, UxMxPxMT);;
	return UxMxPxMT[0]*V[0] + UxMxPxMT[1]*V[1] + UxMxPxMT[2]*V[2] + UxMxPxMT[3]*V[3];
}



void createModel(int tesselation, char *dot3DFile)
{
	Shape s;
	float divisions = 1.0f / tesselation, u, v;

	for (int i=0 ; i<numPatches ; i++)
	{
		for (int j=0 ; j<16 ; j+= 4)
		{
			
		}
	}

	s.writeToFile(dot3DFile);
}

void geraBezier(char *filePath, int tesselation, char *dot3DFile)
{
	readPatchFile(filePath);
	createModel(tesselation, dot3DFile);
}

