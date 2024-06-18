//
// Created by mauricio on 7/11/22.
//

#include <iostream>
#include <string>
#include <fstream>
#include "letters_bag.h"

using namespace std;

int main(int argc, char *argv[]){

    //srand(time(NULL));

    if (argc != 2)
        cout << "Número incorrecto de argumentos";
    else{
        char *archivo = argv[1];
        LettersSet letras;
        LettersBag bolsa;
        int t;

        ifstream ifile;
        ifile.open(archivo);
        if(!ifile){
            cerr << "Error de apertura";
            return 1;
        }

        ifile >> letras;
        bolsa = letras;
        t = bolsa.size();

        for(int i = 0; i < t; t++){
            cout << bolsa.extractLetter();
        }
    }
}