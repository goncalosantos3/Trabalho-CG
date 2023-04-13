#pragma once

#include<vector>
#include<iostream>
#include <fstream>
#include "Point.h"

class Shape
{
private:
	std::string name, file;
	std::vector<Point> pontos;

public:
	Shape():name(""), file(""), pontos(){pontos.reserve(100);};
	Shape(std::string name, std::string filename):name(name), file(filename), pontos() {pontos.reserve(100);};
	Shape(const Shape& shape):name(shape.name), file(shape.file), pontos(shape.pontos){};
	~Shape();

	void addPoint(Point p);
	friend std::ostream& operator<<(std::ostream& stream, const Shape& vec);
	bool operator==(const Shape& shape);;
	void writeToFile(char *file);
	void setName(std::string name);
	void setFile(std::string file);

	std::vector<Point> getPoints();
	std::string getName();
	std::string getFile();
};
