#include "headers/Shape.h"

Shape::~Shape()
{
	pontos.clear();
}

void Shape::addPoint(Point p)
{
	this->pontos.push_back(p);
}

std::ostream& operator<<(std::ostream& stream, const Shape& forma)
{
	for (Point ponto : forma.pontos)
		stream << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";
	return stream;
}

bool Shape::operator==(const Shape &shape)
{
	return this->file == shape.file;
}

void Shape::writeToFile (char *file)
{
	std::string path = "", fileStr = std::string(file);
	if (fileStr.find("../files/") == -1)
		path.append("../files/");
	path.append(fileStr);

	std::ofstream f(path);

	for (Point ponto : this->pontos)
		f << ponto.getX() << "," << ponto.getY() << ", " << ponto.getZ() << "\n";

	f.close();
}

void Shape::setName(std::string name)
{
	this->name = name;
}

void Shape::setFile(std::string file)
{
	this->file = file;
}

std::vector<Point> Shape::getPoints()
{
	return std::vector<Point>(pontos);
}

std::string Shape::getName()
{
	return name;
}

std::string Shape::getFile()
{
	return file;
}
