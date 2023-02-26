#pragma once

#include<vector>
#include<iostream>
#include "Ponto.h"

class Forma
{
private:
	std::vector<Ponto> pontos

public:
	Forma() :pontos(new vector<Ponto>) {};
	~Forma() {};

	void adicionarPonto(Ponto p);
	std::ostream& operator<<(std::ostream& stream, const Forma& vec)
};