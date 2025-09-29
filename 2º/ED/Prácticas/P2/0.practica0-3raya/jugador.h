
#ifndef __JUGADOR__H__
#define __JUGADOR__H__

/* ***************************************** */

#include <iostream>
#include <string>
#include "ficha.h"
#include "tablero.h"

/* ***************************************** */

class Jugador {
  private:
    std::string nombre;     // Nombre del jugador
    Ficha fic;              // Color de la ficha (cruz o circulo)
    int nivel;              // Nivel del jugador

    // M�todos privados que implementan distintas estrategias de juego
    //   Nivel 0 : Juega una persona
    //   Nivel 1 : Juega la CPU de forma muy b�sica
    //   ... podr�amos implementar nuevos niveles m�s "inteligentes"
    void piensa_nivel_0(const Tablero &tab, int &fil, int &col) const;
    void piensa_nivel_1(const Tablero &tab, int &fil, int &col) const;

  public:
    // No existe constructor por defecto. Cuando construimos un objeto de tipo
    // jugador debemos asignarle un nombre y un color obligatoriamente.
    Jugador(const std::string &n, const Ficha &f, int ni);

    //    ~Jugador() { };     // El destructor est� vacio

    // Obtener el nombre del jugador
    std::string Nombre() const         { return nombre; };

    // Obtener el color de la ficha
    Ficha Color() const                { return fic; };

    // Le damos el tablero y nos devuelve d�nde quiere poner ficha el jugador
    void PiensaJugada(const Tablero &tab, int &fil, int &col) const;
};

/* ***************************************** */

// Para mostrar los datos del jugador en consola
std::ostream& operator<<(std::ostream &salida, const Jugador &jug);

/* ***************************************** */

#endif
