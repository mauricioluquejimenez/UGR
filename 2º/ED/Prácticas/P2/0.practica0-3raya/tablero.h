
#ifndef __TABLERO__H__
#define __TABLERO__H__

/* ***************************************** */

#include <iostream>
#include "ficha.h"

/* ***************************************** */

class Tablero {

  private:
    Ficha tab[3][3];      // Tablero de fichas 3x3
    int numfichas;        // Número de fichas que han sido puestas en total

    // Copia un tablero desde orig
    void copia_tablero(const Tablero &orig);

    // Devuelve true/false si la ficha de tipo fic tiene 3 en raya
    bool hay3raya(const Ficha &fic) const;

  public:
    Tablero();                                  // Constructor por defecto
    ~Tablero() { };                             // Destructor (vacío)

    Tablero(const Tablero &orig);               // Constructor de copia
    Tablero& operator=(const Tablero &orig);    // Sobrecarga de asignación

    // Inicializa el tablero poniendo en blanco todas sus casillas
    void PonerEnBlanco();     // No es necesario

    // Pone una ficha de color fic en la fila f y la columna c
    // Devuelve true si la operación ha tenido éxito y false en caso contrario.
    // Sólo se pueden poner fichas en las casillas que estén en blanco
    bool PonFicha(int f, int c, const Ficha &fic);

    // Devuelve el color de la ficha que hay en la posición (f,c)
    Ficha QueFichaHay(int f, int c) const;

    // Devuelve si hay algún color que tenga tres en raya. Si no hay 3 en raya
    // devuelve el valor blanco.
    Ficha Busca3Raya() const;

    // Devuelve el número de fichas que hay puestas en el tablero
    int CuantasFichas() const { return numfichas; };

};

// Para mostrar el tablero en pantalla sobrecargamos <<
std::ostream& operator<<(std::ostream &salida, const Tablero &tab);

/* ***************************************** */

#endif
