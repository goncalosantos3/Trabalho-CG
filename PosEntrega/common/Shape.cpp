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

void Shape::addTextCoords(Point2D p)
{
    this->coordsText.emplace_back(p.getX(), p.getY());
}

std::ostream& operator<<(std::ostream& stream, const Shape& forma)
{
	stream << forma.pontos.size() << "\n";
	for (Point ponto : forma.pontos)
		stream << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";
	for (Point ponto : forma.normais)
		stream << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";
    for (Point2D ponto : forma.coordsText)
        stream << ponto.getX() << " " << ponto.getY() << "\n";
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
    for (Point2D ponto : this->coordsText)
        f << ponto.getX() << ", " << ponto.getY() << "\n";

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

void Shape::normalizeNormals()
{
	for (Point& normal : normais)
		normal.normalize();
}

void Shape::multiplyNormals(float num)
{
    for (int i=0 ; i<normais.size() ; i++)
        normais[i] = normais[i]*num;
}

std::vector<Point> Shape::getPoints()
{
	return std::vector<Point>(pontos);
}

std::vector<Point> Shape::getNormals()
{
	return std::vector<Point>(normais);
}

std::vector<Point2D> Shape::getTextCoords()
{
    return std::vector<Point2D>(coordsText);
}

std::string Shape::getName()
{
	return name;
}

std::string Shape::getFile()
{
	return file;
}
