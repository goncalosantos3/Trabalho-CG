#include "Shape.h"

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
	std::string path = "", fileStr = std::string(file);
	if (!fileStr.find("../files/"))
		path.append("../files/");
	path.append(fileStr);

	std::ofstream f(path);

	for (Point ponto : this->pontos)
		f << ponto.getX() << "," << ponto.getY() << ", " << ponto.getZ() << "\n";

	f.close();
}
