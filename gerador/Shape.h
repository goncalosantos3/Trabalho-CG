#pragma once

#include<vector>
#include<iostream>
#include <fstream>
#include<string>
#include "Point.h"

class Shape
{
private:
	std::vector<Point> pontos;

public:
	Shape() {};
	~Shape() {};

	void addPoint(Point p);
	friend std::ostream& operator<<(std::ostream& stream, const Shape& vec);
	void writeToFile(char *file);
};
