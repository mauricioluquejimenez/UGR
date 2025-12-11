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

bool PalabraValida(const string &palabra, const vector<char> &letras)
{
  bool valida = true;
  char c;
  int frec[26] = {0};

  for (int i = 0; i < (int)letras.size(); i++)
  {
    c = tolower(letras[i]);
    if (c >= 'a' && c <= 'z')
      frec[c - 'a']++;
  }

  // Gastar letras según la palabra
  for (int i = 0; i < palabra.size(); i++)
  {
    c = tolower(palabra[i]);

    // Si no es letra, ya consideramos que es inválida
    if (c < 'a' || c > 'z')
    {
      valida = false;
      break;
    }

    // se ha usado más veces de las que hay
    frec[c - 'a']--;
    if (frec[c - 'a'] < 0)
    {
      valida = false;
      break;
    }
  }

  return valida;
}

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
    return 0;
  }

  ifstream letters_file(argv[2]);
  if (!letters_file)
  {
    cerr << "No puedo abrir el fichero de letras " << argv[2] << endl;
    return 0;
  }

  Dictionary dic; dic_file >> dic;
  LettersSet set; letters_file >> set;
  LettersBag bag;
  Solver solver(dic, set);

  int num_letras = atoi(argv[3]), puntuacion_usuario = -1, mejor_puntuacion = -1;
  char modo = argv[4][0], seguir = 'S';
  bool score_game, valida;
  string palabra_usuario;

  vector<char> letras;
  pair<vector<string>, int> resultado;

  if(modo == 'P' || modo == 'p') score_game = true;
  else if(modo == 'L' || modo == 'l') score_game = false;
  else
  {
    cout << "Modo de juego no válido. Usar L o P." << endl;
    return 0;
  }

  cout << "¡Bienvenido al juego de las letras!" << endl;
  cout << "Modo de juego: " << (score_game ? "Puntuacion" : "Longitud") << endl;

  while(seguir == 'S' || seguir == 's')
  {
    bag = LettersBag(set);
    letras = bag.extractLetters(num_letras);
    resultado = solver.getSolutions(letras, score_game);

    cout << "\nLetras generadas: ";
    for(unsigned int i = 0; i < letras.size(); i++) cout << letras[i] << " "; cout << endl;

    cout << "\nDime tu solucion: "; cin >> palabra_usuario;
    cout << "\nTu solución: " << palabra_usuario;

    valida = PalabraValida(palabra_usuario, letras) && dic.exists(palabra_usuario);
    if(!valida)
    {
      cout << "\nLa palabra no es válida (no está en el diccionario o no se puede formar con las letras dadas)." << endl;
      puntuacion_usuario = 0;
    }
    else
    {
      puntuacion_usuario = score_game ? set.getScore(palabra_usuario) : static_cast<int>(palabra_usuario.size());
      cout << "\nPuntuacion: " << puntuacion_usuario << endl;
    }

    cout << "\nMejores soluciones: " << endl;
    for(unsigned int i = 0; i < resultado.first.size(); i++)
    {
      cout << " " << resultado.first[i];
      if (score_game)
      {
        mejor_puntuacion = set.getScore(resultado.first[i]);
        cout << " (Puntuacion: " << mejor_puntuacion << ")";
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