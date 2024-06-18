#include <fstream>
#include <iostream>

#include "letters_set.h"
#include "solver.h"
#include "letters_bag.h"
#include "dictionary.h"

using namespace std;

int main(int argc, char *argv[]) {
    char *archivo1 = argv[1];
    char *archivo2 = argv[2];
    Dictionary dicc;
    LettersSet letras;
    ifstream f1, f2;
    bool b_mode;
    string modo;
    int cantidad_letras;
    Solver sol(dicc, letras);
    LettersBag bolsa(letras);

    vector<char> letras_dispo;
    pair<vector<string>, int> soluciones;

    if (argc != 5) {
        cout << "Error en el numero de argumentos" << endl;
        exit(-1);
    }

    f1.open(argv[1]);
    f2.open(argv[2]);

    if (!archivo1) {
        cerr << "Error en la apertura del archivo 1";
        return 1;
    }
    if (!archivo2) {
        cerr << "Error en la apertura del archivo 2";
        return 2;
    }


    while (!f1.eof()) {
        string word;
        f1 >> word;
        dicc.insert(word);
    }


    modo = argv[3];
    cantidad_letras = atoi(argv[4]);
    letras_dispo = bolsa.extractLetters(cantidad_letras);

    if (modo == "P") {
        b_mode = true;
    } else if (modo == "L") {
        b_mode = false;
    }
    soluciones = sol.getSolutions(letras_dispo, b_mode);

    cout << "LETRAS DISPONIBLES:" << endl;
    for (int i = 0; i < letras_dispo.size(); i++) {
        cout << toupper(letras_dispo[i]) << " ";
    }

    cout << endl << "SOLUCIONES:" << endl;
    for (int i = 0; i < soluciones.first.size(); i++) {
        cout << soluciones.first[i] << endl;
    }

    cout << "PUNTUACION:" << endl;
    cout << soluciones.second << endl;

}