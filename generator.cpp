#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 
using namespace std;

// A coordenada y é sempre 0
// O plano tem que estar centrado na origem
void geraPlano(float len, float div, char *file){
    ofstream f(file);

    float x, y = 0, z;
    float ladoQua = len/div;
    float ref = len / 2;

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

    f.close();
}

int main(int argc, char *argv[]){

    if(strcmp(argv[1],"plane") == 0){
        geraPlano(stof(argv[2]), stof(argv[3]), argv[4]);
    }

    return 1;
}