#ifndef __LETTERS_BAG_H__
#define __LETTERS_BAG_H__

/**
 * @brief TDA LettersBag
 *
 * Este TDA almacena la bolsa de letras utilizada en el juego de las letras.
 * La bolsa contiene todas las letras indicadas por un LettersSet,
 * repitiéndose cada carácter tantas veces como indique su número de repeticiones.
 *
 * La estructura de datos subyacente es un vector de chars, sobre el cual se
 * realizan extracciones aleatorias de letras.
 */

#include "letters_set.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class LettersBag {
private:
    vector<char> letters;

public:

    /**
     * @brief Constructor por defecto. Crea una bolsa vacía.
     */
    LettersBag();

    /**
     * @brief Constructor a partir de un LettersSet.
     * Rellena la bolsa repitiendo cada letra tantas veces como indique repetitions.
     */
    LettersBag(const LettersSet & letterSet);

    /**
     * @brief Inserta una letra en la bolsa.
     * @param l Letra a añadir a el LettersBag.
     */
    void insertLetter(const char & l);

    /**
     * @brief Extrae una letra aleatoria de la bolsa y la elimina de ella.
     * @return Letra extraída.
     */
    char extractLetter();

    /**
     * @brief Extrae varias letras aleatorias.
     * Extrae un conjunto de letras del LettersBag, y las elimina de la bolsa.
     * @param num Número de letras a extraer.
     * @return Vector con las letras extraídas.
     */
    vector<char> extractLetters(int num);

    /**
     * @brief Vacía la bolsa.
     */
    void clear();

    /**
     * @brief Devuelve el tamaño actual de la bolsa.
     * @return entero con el tamaño de la bolsa.
     */
    unsigned int size() const;

    /**
     * @brief Sobrecarga del operador de asignación.
     * @param other LettersBag a copiar
     * @return Referencia al objeto @p  para  encadenar el operador.
     */
    LettersBag & operator=(const LettersBag & other);
};

#endif
