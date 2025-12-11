#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "dictionary.h"
#include "letters_bag.h"
#include "letters_set.h"
#include "solver.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    cout << "Los parametros son: " << endl;
    cout << "1.- El fichero con el diccionario" << endl;
    cout << "2.- El fichero con las letras" << endl;
    cout << "3.- El numero de letras a generar" << endl;
    cout << "4.- Modo de juego (L/P)" << endl;
    return 0;
  }

  ifstream dic_file(argv[1]);
  if (!dic_file)
  {
    cerr << "No puedo abrir el fichero de diccionario " << argv[1] << endl;
    return 1;
  }

  ifstream letters_file(argv[2]);
  if (!letters_file)
  {
    cerr << "No puedo abrir el fichero de letras " << argv[2] << endl;
    return 1;
  }

  Dictionary dic; dic_file >> dic;
  LettersSet set; letters_file >> set;
  
  Solver solver(dic, set);

  int num_letras = atoi(argv[3]), puntuacion_usuario = -1;
  char modo = argv[4][0], seguir = 'S';
  bool score_game = (modo == 'P' || modo == 'p');
  string palabra_usuario;
  

  while(seguir == 'S' || seguir == 's')
  {
    LettersBag bag(set);

    vector<char> letras = bag.extractLetters(num_letras);
    pair<vector<string>, int> resultado = solver.getSolutions(letras, score_game);

    cout << "\nLetras generadas: ";
    for(unsigned int i = 0; i < letras.size(); i++) cout << letras[i] << " ";

    cout << "\nDime tu solucion: ";
    cin >> palabra_usuario;

    puntuacion_usuario = score_game ? set.getScore(palabra_usuario) : static_cast<int>(palabra_usuario.size());

    cout << "\nTu solución: " << palabra_usuario;
    cout << "\nPuntuacion: " << puntuacion_usuario;

    cout << "\nMejores soluciones: " << endl;
    for(unsigned int i = 0; i < resultado.first.size(); i++)
    {
      cout << " " << resultado.first[i];
      if (score_game)
      {
        int pts = set.getScore(resultado.first[i]);
        cout << " (Puntuacion: " << pts << ")";
      }
      else cout << " (Longitud: " << resultado.first[i].size() << ")";

      cout << '\n';
    }

    cout << "\nPuntuacion/Longitud de la mejor solucion: " << resultado.second << endl;
    cout << "\n¿Desea generar otra partida? (S/N): ";
    cin >> seguir;
  }

  return 0;  
}