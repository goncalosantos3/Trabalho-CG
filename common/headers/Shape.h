#pragma once

#include<vector>
#include<iostream>
#include <fstream>
#include "Point.h"
#include "Point2D.h"

class Shape
{
private:
	std::string name, file;
	std::vector<Point> pontos, normais;
    std::vector<Point2D> coordsText;
	int vboStartIndex, vboStopIndex;

public:
	Shape():name(""), file(""), pontos(), normais(), coordsText(){pontos.reserve(100);normais.reserve(100);coordsText.reserve(100);};
	Shape(std::string name, std::string filename):name(name), file(filename), pontos(),normais(), coordsText() {pontos.reserve(100); normais.reserve(100); coordsText.reserve(100);};
	Shape(const Shape& shape):name(shape.name), file(shape.file), pontos(shape.pontos), normais(shape.normais), coordsText(shape.coordsText), vboStartIndex(shape.vboStartIndex), vboStopIndex(shape.vboStopIndex){};
	~Shape();

	void addPoint(Point p);
	void addNormal(Point p);
    void addTextCoords(Point2D p);
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
    std::vector<Point2D> getTextCoords();
	std::string getName();
	std::string getFile();
	int getVBOStartIndex();
	int getVBOStopIndex();
};
