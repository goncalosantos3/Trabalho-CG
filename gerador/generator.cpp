#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "Shape.h"
#include "Point.h"
using namespace std;

// A coordenada y é sempre 0
// O plano tem que estar centrado na origem
void geraPlano(float len, float div, char *file){
    Shape* forma = new Shape();

    float ref = len / 2;
    float x = -ref, y = 0, z = -ref;
    float ladoQua = len/div;
	int i,j;

    for (i=0, z=-ref ; i < div; i++, z+=ladoQua)
    {
		for (j=0, x=-ref ; j<div ; j++, x+=ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y, z+ladoQua));
            forma->addPoint(Point(x+ladoQua, y, z+ladoQua));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
            forma->addPoint(Point(x+ladoQua, y, z));
        }
    }

    forma->writeToFile(file);
}

// Um cubo é composto por 6 faces
void geraCubo(float len, float div, char *file){
    Shape* forma = new Shape();

    float ref = len / 2;
    float x = -ref, y = -ref, z = -ref;
    float ladoQua = len/div;

    for (int i=0; i < div; i++,z += ladoQua)
    {
        x = -ref;
        for (int j=0; j<div ; j++,x += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x+ladoQua, y, z+ladoQua));
            forma->addPoint(Point(x, y, z+ladoQua));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x+ladoQua, y, z));
            forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
        }
    }

    x = -ref;
    y = ref;
    z = -ref;

    for (int i=0; i<div ; i++,z += ladoQua)
    {
        x = -ref;
        for (int j=0; j<div ; j++, x += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y, z+ladoQua));
            forma->addPoint(Point(x+ladoQua, y, z+ladoQua));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
            forma->addPoint(Point(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (int i=0; i<div ; i++,z += ladoQua)
    {
        y = -ref;
        for (int j=0; j<div ; j++, y += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y, z+ladoQua));
            forma->addPoint(Point(x, y+ladoQua, z+ladoQua));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y+ladoQua, z + ladoQua));
            forma->addPoint(Point(x, y+ladoQua, z));
        }
    }

    x = ref;
    y = -ref;
    z = -ref;

    for (int i=0; i<div ; i++,z += ladoQua)
    {
        y = -ref;
        for (int j=0; j<div ; j++,y += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y+ladoQua, z+ladoQua));
            forma->addPoint(Point(x, y, z+ladoQua));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y+ladoQua, z));
            forma->addPoint(Point(x, y+ladoQua, z + ladoQua));
        }
    }

    x = -ref;
    y = -ref;
    z = -ref;

    for (int i=0; i<div ; i++,x += ladoQua)
    {
        y = -ref;
        for (int j=0; j<div ; j++,y += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x, y+ladoQua, z));
            forma->addPoint(Point(x+ladoQua, y+ladoQua, z));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
            forma->addPoint(Point(x+ladoQua, y, z));
        }
    }

    x = -ref;
    y = -ref;
    z = ref;

    for (int i=0; i<div ; i++,x += ladoQua)
    {
        y = -ref;
        for (int j=0; j<div ; j++,y += ladoQua)
        {
            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
            forma->addPoint(Point(x, y+ladoQua, z));

            forma->addPoint(Point(x, y, z));
            forma->addPoint(Point(x+ladoQua, y, z));
            forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
        }
    }


    forma->writeToFile(file);
}

// A base do cone tem que estar contida no plano XZ
void geraCone(float radius, float height, int slices, int stacks, char * file){
    Shape* forma = new Shape();

    float alfa = 2 * M_PI/slices;
	float alfainc = alfa;
    float alt = height / stacks;
    float altinc = alt;
    float raio = radius / stacks;
    float r = radius - raio;

    for(int i = 0; i < slices; i++){
        // Base
        forma->addPoint(Point(0,0,0));
        forma->addPoint(Point(radius * sin(alfainc),0, radius * cos(alfainc)));
        forma->addPoint(Point(radius * sin(alfainc-alfa),0,radius * cos(alfainc-alfa)));

        // Lados
        for(int j = 0; j < stacks; j++){
            forma->addPoint(Point((r + raio) * sin(alfainc),altinc-alt,(r + raio) * cos(alfainc)));
            forma->addPoint(Point(r * sin(alfainc),altinc,r * cos(alfainc)));
            forma->addPoint(Point(r * sin(alfainc-alfa),altinc,r * cos(alfainc-alfa)));
            
            forma->addPoint(Point((r + raio) * sin(alfainc-alfa),altinc-alt,(r + raio) * cos(alfainc-alfa)));
            forma->addPoint(Point((r + raio) * sin(alfainc),altinc-alt,(r + raio) * cos(alfainc)));
            forma->addPoint(Point(r * sin(alfainc-alfa),altinc,r * cos(alfainc-alfa)));

            altinc += alt;
            r -= raio;
        }

        alfainc += alfa;
        altinc = alt;
        r = radius - raio;
    }

    forma->writeToFile(file);
}

void geraEsfera(float radius, int slices, int stacks, char *file) {
    Shape* forma = new Shape();


    float alphainc = 2 * M_PI / slices;
    float alpha=0;
    float betainc = M_PI / stacks;
    float beta=-M_PI_2;

    for (int i = 0; i < stacks; i++, beta+=betainc) {
        alpha = alphainc/2;
        for (int j = 0; j < slices; j++, alpha+=alphainc) {
            forma->addPoint(Point(radius * cos(beta) * cos(alpha), radius * sin(beta), radius * cos(beta) * sin(alpha)));
            forma->addPoint(Point(radius * cos(beta + betainc) * cos(alpha + alphainc), radius * sin(beta + betainc), radius * cos(beta + betainc) * sin(alpha + alphainc)));
            forma->addPoint(Point(radius * cos(beta) * cos(alpha + alphainc), radius * sin(beta), radius * cos(beta) * sin(alpha + alphainc)));

            forma->addPoint(Point(radius * cos(beta + betainc) * cos(alpha), radius * sin(beta + betainc), radius * cos(beta + betainc) * sin(alpha)));
            forma->addPoint(Point(radius * cos(beta + betainc) * cos(alpha + alphainc), radius * sin(beta + betainc), radius * cos(beta + betainc) * sin(alpha + alphainc)));
            forma->addPoint(Point(radius * cos(beta) * cos(alpha), radius * sin(beta), radius * cos(beta) * sin(alpha)));
        }
    }

    forma->writeToFile(file);
}


int main(int argc, char *argv[]){

    if(strcmp(argv[1],"plane") == 0){
        geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
    }else if(strcmp(argv[1],"box") == 0){
        geraCubo(stof(argv[2]),stof(argv[3]),argv[4]);
    }else if(strcmp(argv[1],"cone") == 0){
        geraCone(stof(argv[2]),stof(argv[3]),stoi(argv[4]),stoi(argv[5]),argv[6]);
    }
	else if(strcmp(argv[1],"sphere") == 0){
		geraEsfera(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), argv[5]);
	}else{
        printf("Primitiva: %s, não suportada!\n", argv[1]);
    }

    return 0;
}
