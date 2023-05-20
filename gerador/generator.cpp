#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sys/stat.h>

#include "../common/headers/Shape.h"
#include "../common/headers/Point.h"
#include "headers/Bezier.h"

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
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x, y, z+ladoQua));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x+ladoQua, y, z+ladoQua));
			forma->addNormal(Point(0, 1, 0));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x+ladoQua, y, z));
			forma->addNormal(Point(0, 1, 0));
        }
	}

	forma->normalizeNormals();
	forma->writeToFile(file);
}

// Um cubo é composto por 6 faces
void geraCubo(float len, float div, char *file){
	Shape* forma = new Shape();

	float ref = len / 2;
	float x = -ref, y = -ref, z = -ref;
	float ladoQua = len/div;

	// face de baixo
	for (int i=0; i < div; i++,z += ladoQua)
	{
		x = -ref;
		for (int j=0; j<div ; j++,x += ladoQua)
		{
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, -1, 0));
			forma->addPoint(Point(x+ladoQua, y, z+ladoQua));
			forma->addNormal(Point(0, -1, 0));
			forma->addPoint(Point(x, y, z+ladoQua));
			forma->addNormal(Point(0, -1, 0));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, -1, 0));
			forma->addPoint(Point(x+ladoQua, y, z));
			forma->addNormal(Point(0, -1, 0));
			forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
			forma->addNormal(Point(0, -1, 0));
		}
	}

	// face de cima
	x = -ref;
	y = ref;
	z = -ref;
	for (int i=0; i<div ; i++,z += ladoQua)
	{
		x = -ref;
		for (int j=0; j<div ; j++, x += ladoQua)
		{
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x, y, z+ladoQua));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x+ladoQua, y, z+ladoQua));
			forma->addNormal(Point(0, 1, 0));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x + ladoQua, y, z + ladoQua));
			forma->addNormal(Point(0, 1, 0));
			forma->addPoint(Point(x+ladoQua, y, z));
			forma->addNormal(Point(0, 1, 0));
		}
	}

	x = -ref;
	y = -ref;
	z = -ref;

	// face da esquerda
	for (int i=0; i<div ; i++,z += ladoQua)
	{
		y = -ref;
		for (int j=0; j<div ; j++, y += ladoQua)
		{
			forma->addNormal(Point(-1, 0, 0));
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(-1, 0, 0));
			forma->addPoint(Point(x, y, z+ladoQua));
			forma->addNormal(Point(-1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z+ladoQua));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(-1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z + ladoQua));
			forma->addNormal(Point(-1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z));
			forma->addNormal(Point(-1, 0, 0));
		}
	}

	x = ref;
	y = -ref;
	z = -ref;

	// face da direita
	for (int i=0; i<div ; i++,z += ladoQua)
	{
		y = -ref;
		for (int j=0; j<div ; j++,y += ladoQua)
		{
			forma->addNormal(Point(1, 0, 0));
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z+ladoQua));
			forma->addNormal(Point(1, 0, 0));
			forma->addPoint(Point(x, y, z+ladoQua));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z));
			forma->addNormal(Point(1, 0, 0));
			forma->addPoint(Point(x, y+ladoQua, z + ladoQua));
			forma->addNormal(Point(1, 0, 0));
		}
	}

	x = -ref;
	y = -ref;
	z = -ref;
	
	// face de trás
	for (int i=0; i<div ; i++,x += ladoQua)
	{
		y = -ref;
		for (int j=0; j<div ; j++,y += ladoQua)
		{
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 0, -1));
			forma->addPoint(Point(x, y+ladoQua, z));
			forma->addNormal(Point(0, 0, -1));
			forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
			forma->addNormal(Point(0, 0, -1));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 0, -1));
			forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
			forma->addNormal(Point(0, 0, -1));
			forma->addPoint(Point(x+ladoQua, y, z));
			forma->addNormal(Point(0, 0, -1));
		}
	}

	x = -ref;
	y = -ref;
	z = ref;

	// face da frente
	for (int i=0; i<div ; i++,x += ladoQua)
	{
		y = -ref;
		for (int j=0; j<div ; j++,y += ladoQua)
		{
			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 0, 1));
			forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
			forma->addNormal(Point(0, 0, 1));
			forma->addPoint(Point(x, y+ladoQua, z));
			forma->addNormal(Point(0, 0, 1));

			forma->addPoint(Point(x, y, z));
			forma->addNormal(Point(0, 0, 1));
			forma->addPoint(Point(x+ladoQua, y, z));
			forma->addNormal(Point(0, 0, 1));
			forma->addPoint(Point(x+ladoQua, y+ladoQua, z));
			forma->addNormal(Point(0, 0, 1));
		}
	}


	forma->normalizeNormals();
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
		forma->addNormal(Point(0,-1,0));
		forma->addPoint(Point(radius * sin(alfainc),0, radius * cos(alfainc)));
		forma->addNormal(Point(0,-1,0));
		forma->addPoint(Point(radius * sin(alfainc-alfa),0,radius * cos(alfainc-alfa)));
		forma->addNormal(Point(0,-1,0));

		// Lados
		for(int j = 0; j < stacks; j++){
			Point p1 = Point((r + raio) * sin(alfainc),altinc-alt,(r + raio) * cos(alfainc)),
						p2 = Point(r * sin(alfainc),altinc,r * cos(alfainc)),
						p3 = Point(r * sin(alfainc-alfa),altinc,r * cos(alfainc-alfa)),
						p4 = Point((r + raio) * sin(alfainc-alfa),altinc-alt,(r + raio) * cos(alfainc-alfa)),
						n1 = (p2-p1) * (p4-p1),
						n2 = (p3-p2) * (p1-p2),
						n3 = (p4-p3) * (p2-p3),
						n4 = (p1-p4) * (p3-p4);
			forma->addPoint(p1);
			forma->addNormal(n1);
			forma->addPoint(p2);
			forma->addNormal(n2);
			forma->addPoint(p3);
			forma->addNormal(n3);
		    
			forma->addPoint(p4);
			forma->addNormal(n4);
			forma->addPoint(p1);
			forma->addNormal(n1);
			forma->addPoint(p3);
			forma->addNormal(n3);

			altinc += alt;
			r -= raio;
		}

		alfainc += alfa;
		altinc = alt;
		r = radius - raio;
	}

	forma->normalizeNormals();
	forma->writeToFile(file);
}

void geraEsfera(float radius, int slices, int stacks, char *file) {
	Shape* forma = new Shape();


	float alphainc = 2 * M_PI / slices;
	float alpha=0;
	float betainc = M_PI / stacks;
	float beta=-M_PI_2;

	for (int i = 0; i < stacks; i++, beta+=betainc) {
		alpha = 0;
		for (int j = 0; j < slices; j++, alpha+=alphainc) {
			Point p1 = Point(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha)),
						p2 = Point(radius * cos(beta) * sin(alpha + alphainc), radius * sin(beta), radius * cos(beta) * cos(alpha + alphainc)),
						p3 = Point(radius * cos(beta + betainc) * sin(alpha), radius * sin(beta + betainc), radius * cos(beta + betainc) * cos(alpha)),
						p4 = Point(radius * cos(beta + betainc) * sin(alpha + alphainc), radius * sin(beta + betainc), radius * cos(beta + betainc) * cos(alpha + alphainc)),
						n1 = (p2-p1) * (p3-p2),
						n2 = (p2-p4) * (p1-p2),
						n3 = (p1-p3) * (p4-p3),
						n4 = (p3-p4) * (p2-p4);
			forma->addPoint(p1);
			forma->addNormal(n1);
			forma->addPoint(p2);
			forma->addNormal(n2);
			forma->addPoint(p3);
			forma->addNormal(n3);

			forma->addPoint(p4);
			forma->addNormal(n4);
			forma->addPoint(p3);
			forma->addNormal(n3);
			forma->addPoint(p2);
			forma->addNormal(n2);
		}
	}

	forma->normalizeNormals();
	forma->writeToFile(file);
}

void geraCilindro(float radius, float height, int sides, char* file) 
{
	int i;
	float step;

	step = 360.0 / sides;

	Shape *forma = new Shape();

	for (i = 0; i < sides; i++) {
		// top
		forma->addPoint(Point(0, height*0.5, 0));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius));
		forma->addNormal(Point(0,1,0));

		//bottom
		forma->addPoint(Point(0, -height*0.5, 0));
		forma->addNormal(Point(0,-1,0));
		forma->addPoint(Point(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius));
		forma->addNormal(Point(0,-1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius));
		forma->addNormal(Point(0,-1,0));

	// body
		Point p1 = Point(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius),
					p2 = Point(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius),
					p3 = Point(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius),
					p4 = Point(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius),
					n1 = (p2-p1) * (p3-p1),
					n2 = (p4-p2) * (p1-p2),
					n3 = (p1-p3) * (p4-p3),
					n4 = (p3-p4) * (p2-p4);
		forma->addPoint(p1);
		forma->addNormal(n1);
		forma->addPoint(p2);
		forma->addNormal(n2);
		forma->addPoint(p3);
		forma->addNormal(n3);

		forma->addPoint(p2);
		forma->addNormal(n2);
		forma->addPoint(p4);
		forma->addNormal(n4);
		forma->addPoint(p3);
		forma->addNormal(n3);
	}


	forma->normalizeNormals();
	forma->writeToFile(file);
}

void geraArruela(float radiusIn, float radiusOut, float height, int sides, char* file)
{
	int i;
	float step;

	step = 360.0 / sides;

	Shape *forma = new Shape();

	for (i = 0; i < sides; i++) {
		// top
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusIn, height*0.5, -sin(i * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusOut, height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusIn, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusIn, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusOut, height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusOut, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));

		//bottom
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusIn, -height*0.5, -sin(i * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusIn, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusOut, -height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusIn, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos((i+1) * step * M_PI / 180.0)*radiusOut, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));
		forma->addPoint(Point(cos(i * step * M_PI / 180.0)*radiusOut, -height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut));
		forma->addNormal(Point(0,1,0));

	// body out
		Point p1 = Point(cos(i * step * M_PI / 180.0)*radiusOut, height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut),
					p2 = Point(cos(i * step * M_PI / 180.0)*radiusOut, -height*0.5, -sin(i * step *M_PI / 180.0)*radiusOut), 
					p3 = Point(cos((i + 1) * step * M_PI / 180.0)*radiusOut, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusOut),
					p4 = Point(cos((i + 1) * step * M_PI / 180.0)*radiusOut, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusOut),
					n1 = (p2-p1) * (p3-p1),
					n2 = (p4-p2) * (p1-p2),
					n3 = (p1-p3) * (p4-p3),
					n4 = (p3-p4) * (p2-p4);
		forma->addPoint(p1);
		forma->addNormal(n1);
		forma->addPoint(p2);
		forma->addNormal(n2);
		forma->addPoint(p3);
		forma->addNormal(n3);

		forma->addPoint(p2);
		forma->addNormal(n2);
		forma->addPoint(p4);
		forma->addNormal(n4);
		forma->addPoint(p3);
		forma->addNormal(n3);

		Point p5 = Point(cos(i * step * M_PI / 180.0)*radiusIn, height*0.5, -sin(i * step *M_PI / 180.0)*radiusIn),
					p6 = Point(cos(i * step * M_PI / 180.0)*radiusIn, -height*0.5, -sin(i * step *M_PI / 180.0)*radiusIn), 
					p7 = Point(cos((i + 1) * step * M_PI / 180.0)*radiusIn, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn),
					p8 = Point(cos((i + 1) * step * M_PI / 180.0)*radiusIn, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radiusIn),
					n5 = (p7-p5) * (p6-p5),
					n6 = (p5-p6) * (p8-p6),
					n7 = (p8-p7) * (p5-p7),
					n8 = (p6-p8) * (p7-p8);
	// body in
		forma->addPoint(p5);
		forma->addNormal(p5);
		forma->addPoint(p7);
		forma->addNormal(p7);
		forma->addPoint(p6);
		forma->addNormal(p6);

		forma->addPoint(p6);
		forma->addNormal(p6);
		forma->addPoint(p7);
		forma->addNormal(p7);
		forma->addPoint(p8);
		forma->addNormal(p8);
	}


	forma->normalizeNormals();
	forma->writeToFile(file);
}


int main(int argc, char *argv[]){

	if(strcmp(argv[1],"plane") == 0){
		geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
	}else if(strcmp(argv[1],"box") == 0){
		geraCubo(stof(argv[2]),stof(argv[3]),argv[4]);
	}else if(strcmp(argv[1],"cone") == 0){
		geraCone(stof(argv[2]),stof(argv[3]),stoi(argv[4]),stoi(argv[5]),argv[6]);
	}else if(strcmp(argv[1],"sphere") == 0){
		geraEsfera(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), argv[5]);
	}else if(strcmp(argv[1], "cylinder")==0){
		geraCilindro(stof(argv[2]), stof(argv[3]), stoi(argv[4]), argv[5]);
	}else if(strcmp(argv[1], "washer")==0){
		geraArruela(stof(argv[2]), stof(argv[3]), stof(argv[4]), stoi(argv[5]), argv[6]);
	}else if(strcmp(argv[1], "bezier")==0){
		geraBezier(argv[2], stoi(argv[3]), argv[4]);
	}
	else{
		printf("Primitiva: %s, não suportada!\n", argv[1]);
	}

	return 0;
}
