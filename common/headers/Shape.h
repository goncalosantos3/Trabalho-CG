#pragma once

#include<vector>
#include<iostream>
#include <fstream>
#include "Point.h"

class Shape
{
private:
	std::string name, file;
	std::vector<Point> pontos, normais;
	int vboStartIndex, vboStopIndex;

public:
	Shape():name(""), file(""), pontos(), normais(){pontos.reserve(100);normais.reserve(100);};
	Shape(std::string name, std::string filename):name(name), file(filename), pontos(),normais() {pontos.reserve(100); normais.reserve(100);};
	Shape(const Shape& shape):name(shape.name), file(shape.file), pontos(shape.pontos), normais(shape.normais), vboStartIndex(shape.vboStartIndex), vboStopIndex(shape.vboStopIndex){
		printf("Duplicated Shape\n");
	};
	~Shape();

	void addPoint(Point p);
	void addNormal(Point p);
	friend std::ostream& operator<<(std::ostream& stream, const Shape& vec);
	bool operator==(const Shape& shape);;
	void writeToFile(char *file);
	void setName(std::string name);
	void setFile(std::string file);
	void setVBOStartIndex(int idx);
	void setVBOStopIndex(int idx);
	void normalizeNormals();

	std::vector<Point> getPoints();
	std::vector<Point> getNormals();
	std::string getName();
	std::string getFile();
	int getVBOStartIndex();
	int getVBOStopIndex();
};
