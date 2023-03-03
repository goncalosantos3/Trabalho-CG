#include "headers/Shape.h"

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

void Shape::writeToFile (char *file)
{
	std::ofstream f(file);

	for (Point ponto : this->pontos)
		f << ponto.getX() << ", " << ponto.getY() << ", " << ponto.getZ() << "\n";

	f.close();
}

std::vector<Point> Shape::getPoints()
{
	return this->pontos;
}
