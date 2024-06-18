#ifndef __LETTERS_BAG_H__
#define __LETTERS_BAG_H__

/**
 * @brief TDA LettersBag
 *
 * Este TDA almacena un conjunto de chars utilizado en el juego de letras.
 * La estructura de datos subyacente es una lista de chars.
 */

#include <iostream>
#include <bag.h>
#include <letters_set.h>

class LettersBag{
private:
    Bag<char> letters;

public:
    /**
     * @brief Constructor por defecto.
     * Crea un LettersBag.
     */
    LettersBag();
    /**
    * @brief Constructor  de LettersSet.
    * Pasado LettersSet como argumento el constructor rellena el LettersBag con las letras  del LetterSet,
    * metiendo cada letra el número de veces indicado.
    * @param letterSet TDA LettersSet.
    */
    LettersBag(const LettersSet & letterSet);
    /**
     * @brief Introduce la letra indicada en la bolsa.
     * @param l Letra a añadir a el LettersBag.
     */
    void insertLetter(const char & l);
    /**
    * @brief Extrae una letra aleatoria de la bolsa.
    * Extrae una letra aleatoria de la bolsa, eliminándola del conjunto.
    * @return La letra extraida del conjunto.
    */
    char extractLetter();
    /**
     * @brief Extrae un número concreto de letras de la bolsa.
     * Extrae un conjunto de letras del LettersBag, y las elimina de la bolsa.
     * @param num Número de letras a extraer.
     * @return Lista de letras extraidas aleatoriamente.
     */
    vector<char> extractLetters(int num);
    /**
     * @brief Vacía el LettersBag.
     * Elimina todo lo de la bolsa.
     */
    void clear();
    /**
     * @brief Tamaño de la bolsa.
     * @return entero con el tamaño de la bolsa.
     */
    unsigned int size() const;
    /**
   * @Brief Sobrecarga del operador de asignación.
   * @param other LettersBag a copiar
   * @return Referencia al objeto @p  para  encadenar el operador.
   */
    LettersBag & operator = (const LettersBag & other);
};
#endif
