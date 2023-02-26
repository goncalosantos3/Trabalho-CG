#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Forma.h"
#include "Ponto.h"
using namespace std;

// A coordenada y é sempre 0
// O plano tem que estar centrado na origem
void geraPlano(float len, float div, char *file){
    // ofstream f(file);

    Forma* forma = new Forma();

    float ref = len / 2;
    float x = -ref, y = 0, z = -ref;
    float ladoQua = len/div;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    forma->escreveFicheiro(file);
}

// Um cubo é composto por 6 faces
void geraCubo(float len, float div, char *file){
    Forma* forma = new Forma();

    float ref = len / 2;
    float x = -ref, y = -ref, z = -ref;
    float ladoQua = len/div;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
        }
    }

    x = -ref;
    y = ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        x = -ref;
        for (; x < ref; x += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x + ladoQua, y, z + ladoQua));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z + ladoQua));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
        }
    }

    x = ref;
    y = -ref;
    z = -ref;

    for (; z < ref; z += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z+ladoQua));
            forma->adicionarPonto(Ponto(x, y, z+ladoQua));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z + ladoQua));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (; x < ref; x += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = ref;

    for (; x < ref; x += ladoQua)
    {
        y = -ref;
        for (; y < ref; y += ladoQua)
        {
            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
            forma->adicionarPonto(Ponto(x, y+ladoQua, z));

            forma->adicionarPonto(Ponto(x, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y, z));
            forma->adicionarPonto(Ponto(x+ladoQua, y+ladoQua, z));
        }
    }


    forma->escreveFicheiro(file);
}

int main(int argc, char *argv[]){

    if(strcmp(argv[1],"plane") == 0){
        geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
    }else if(strcmp(argv[1],"box") == 0){
        geraCubo(stof(argv[2]),stof(argv[3]),argv[4]);
    }

    return 1;
}