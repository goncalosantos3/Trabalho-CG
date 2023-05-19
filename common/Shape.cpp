#include "headers/Shape.h"

Shape::~Shape()
{
	pontos.clear();
}

void Shape::addPoint(Point p)
{
	this->pontos.emplace_back(p.getX(), p.getY(), p.getZ());
}

void Shape::addNormal(Point p)
{
	this->normais.emplace_back(p.getX(), p.getY(), p.getZ());
}

std::ostream& operator<<(std::ostream& stream, const Shape& forma)
{
	stream << forma.pontos.size() << "\n";
	for (Point ponto : forma.pontos)
		stream << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";
	for (Point ponto : forma.normais)
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

	f << pontos.size() << "\n";
	for (Point ponto : this->pontos)
		f << ponto.getX() << "," << ponto.getY() << ", " << ponto.getZ() << "\n";
	for (Point ponto : this->normais)
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

void Shape::setVBOStartIndex(int idx)
{
	this->vboStartIndex = idx;
}

void Shape::setVBOStopIndex(int idx)
{
	this->vboStopIndex = idx;
}

void Shape::normalizeNormals()
{
	for (Point& normal : normais)
		normal.normalize();
}

std::vector<Point> Shape::getPoints()
{
	return std::vector<Point>(pontos);
}

std::vector<Point> Shape::getNormals()
{
	return std::vector<Point>(normais);
}

std::string Shape::getName()
{
	return name;
}

std::string Shape::getFile()
{
	return file;
}

int Shape::getVBOStartIndex()
{
	return vboStartIndex;
}

int Shape::getVBOStopIndex()
{
	return vboStopIndex;
}
