#ifndef __SOLVER_H__
#define __SOLVER_H__

#include <string>
#include <utility>
#include <vector>

#include "letters_set.h"
#include "dictionary.h"

using namespace std;

class Solver{
private:
  Dictionary dictionary;
  LettersSet letters_set;

public:
  /**
   * @brief COnstructor con parámetros, se crea un solver con sus respectivos Diccionario y Letters_set
   * @param dict Diccionario que asociaremos al solver
   * @param letters_set Conjunto de letras que asociaremos al solver
   */
  Solver(const Dictionary & dict, const LettersSet & letters_set);
  
  /**
   * @brief Construye el vector de las soluciones al juego partiendo de las letras dado el vector de letras de una partida. Conociendo las letras y el modo de juego de la partida se construye un vector con las soluciones encontradas escogiendo la mejor de las soluciones.
   * @param available_letters vector de letras disponibles a usar en la partida
   * @param score_game tipo de partida. Si es true la partida es a puntuación; en caso contrario se juega a longitud.
   * @return Devuelve un par de <vector<string>, int>, donde el entero será la puntuación de la partida y el vector contendrá las mejores soluciones de la partida.
   */
  pair<vector<string>, int> getSolutions(const vector<char> & available_letters, bool score_game);
};

#endif // __SOLVER_H__
