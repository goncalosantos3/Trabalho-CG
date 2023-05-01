#include "headers/Bezier.h"

using namespace std;

int numPatches, numControlPoints;
vector<vector<int>> indices;
vector<Point> controlPoints;

int divs = 32;

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

Point getBezierPoint (float t, Point P[4])
{
	float t2 = t*t, t3 = t2*t;
	float b0 = -t3 + 3*t2 - 3*t + 1,
				b1 = 3*t3 -6*t2 + 3*t,
				b2 = -3*t3 + 3*t2,
				b3 = t3;

	return P[0]*b0 + P[1]*b1 + P[2]*b2 + P[3]*b3; 
}

Point getGlobalBezierPoint (float t, int curveIndex)
{
	vector<Point> points, aux;
	points.reserve(16);
	for (int i : indices.at(curveIndex))
		points.push_back(controlPoints.at(i));

	while(points.size() > 4)
	{
		int auxSize = points.size()-1;
		aux.reserve(auxSize);

		for (int i=0 ; i<auxSize-3 ; i++)
		{
			Point P[4] = 	{ points.at(i), 
											points.at(i+1),
											points.at(i+2),
											points.at(i+3) };
			aux.emplace_back(getBezierPoint(t, P));
		}
		points = aux;
		aux = vector<Point>();
	}

	Point P[4] = 	{ points.at(0), 
									points.at(1),
									points.at(2),
									points.at(3) };

	return getBezierPoint(t, P);
}

void createModel(char *dot3DFile)
{
	vector<vector<Point>> bezierPoints;
	bezierPoints.reserve(numPatches);

	for (int i=0 ; i<numPatches ; i++)
	{
		bezierPoints.push_back(vector<Point>());
		bezierPoints.at(i).reserve(16);
		float t = 0, inc = 1.0f / 16;
		for (int j=0 ; j<16 ; j++, t+=inc)
			bezierPoints.at(i).emplace_back(getGlobalBezierPoint(t, i));
	}
	
	Shape s;
	for (int i=0 ; i<numPatches-1 ; i++)
	{
		for (int j=0 ; j<15 ; j++)
		{
			s.addPoint(bezierPoints.at(i).at(j));
			s.addPoint(bezierPoints.at(i+1).at(j+1));
			s.addPoint(bezierPoints.at(i).at(j+1));
			s.addPoint(bezierPoints.at(i+1).at(j));
			s.addPoint(bezierPoints.at(i+1).at(j+1));
		}
	}

	s.writeToFile(dot3DFile);
}

void geraBezier(char *filePath, char *dot3DFile)
{
	readPatchFile(filePath);
	createModel(dot3DFile);
}

