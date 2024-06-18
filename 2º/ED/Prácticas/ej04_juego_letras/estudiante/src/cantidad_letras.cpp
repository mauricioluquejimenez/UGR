#include <fstream>
#include <iostream>

#include "letters_set.h"
#include "dictionary.h"

using namespace std;

int main(int argc , char ** argv){

    char *archivo1 = argv[1];
    char *archivo2 = argv[2];
    Dictionary dicc;
    LettersSet letras;
    double total_letras;

    ifstream f1 , f2;

    f1.open(argv[1]);
    f2.open(argv[2]);

    if(!archivo1){
        cerr << "Error en la apertura del archivo 1";
        return 1;
    }
    if(!archivo2){
        cerr << "Error en la apertura del archivo 2";
        return 2;
    }


    while (!f1.eof()){
        string word;
        f1 >> word;
        dicc.insert(word);
    }

    f2 >> letras;

    LettersSet::const_iterator it;

    cout << "Letra" << "\t" << "FAbs." << "\t" << "Frel." << endl;
    for(it = letras.cbegin(); it != letras.cend(); it++ ){
        char letra = (*it).first;
        int ocurrencias = dicc.getTotalUsages(letra);
        int total_usos= dicc.getTotalUsages(letra);
        cout << (*it).first << total_usos << ocurrencias << endl;
    }

}