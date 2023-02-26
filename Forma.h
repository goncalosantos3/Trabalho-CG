#pragma once

#include<vector>
#include<iostream>
#include <fstream>
#include "Ponto.h"

class Forma
{
private:
	std::vector<Ponto> pontos;

public:
	Forma() {};
	~Forma() {};

	void adicionarPonto(Ponto p);
	friend std::ostream& operator<<(std::ostream& stream, const Forma& vec);
	void escreveFicheiro(char *file);
};