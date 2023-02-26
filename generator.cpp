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
    ofstream f(file);

    Forma forma = new Forma();

    float ref = len / 2;
    float x = -ref, y = 0, z = -ref;
    float ladoQua = len/div;

    for (; z < ref; z += ladoQua)
    {
        for (; x < ref; x += ladoQua)
        {
            forma->addPonto(new Ponto(x, y, z));
            forma->addPonto(new Ponto(x, y, z+ladoQua));
            forma->addPonto(new Ponto(x+ladoQua, y, z+ladoQua));

            forma->addPonto(new Ponto(x, y, z));
            forma->addPonto(new Ponto(x + ladoQua, y, z + ladoQua));
            forma->addPonto(new Ponto(x+ladoQua, y, z));
        }
    }

    /*
    for(float i = 0; i < div; i++){
        for(float j = 0; j < div; j++){
            for(float k = 0; k < 2; k ++){
                x = -ref + j * ladoQua;
                z = -ref + j * ladoQua;
                f << to_string(x) + ", " + to_string(y) + ", " + to_string(z) + "\n";

                x = -ref + j * ladoQua;
                z = -ref + j * ladoQua;
                f << to_string(x) + ", " + to_string(y) + ", " + to_string(z) + "\n";


                f << to_string(x) + ", " + to_string(y) + ", " + to_string(z) + "\n";
            }
        }
    }
    */

    f << forma;
    f.close();
}

int main(int argc, char *argv[]){

    if(strcmp(argv[1],"plane") == 0){
        geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
    }

    return 1;
}