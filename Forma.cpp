#include "Forma.h"

void Forma::adicionarPonto(Ponto p)
{
	this->pontos.push_back(Ponto);
}

std::ostream& Forma::operator<<(std::ostream& stream, const Forma& vec)
{
	for (Ponto ponto : this->pontos)
		stream << ponto.getX() << " " << ponto.getY << " " << ponto.getZ() << "\n";
	return stream;
}