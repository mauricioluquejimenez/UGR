
#ifndef __JUGADOR__H__
#define __JUGADOR__H__

/* ***************************************** */

#include <iostream>
#include <string>

#include "tablero.h"
/**
  @brief TDA Jugador
  @author Rosa Rodríguez-Sanchez
  @date 3/10/2025
**/
/* ***************************************** */

class Jugador {
  private:
    std::string nombre;   // Nombre del jugador
    
    // Métodos privados que implementan distintas estrategias de juego
    //   Nivel 0 : Juega una persona
    //   Nivel 1 : Juega la CPU de forma muy básica
    //   ... podríamos implementar nuevos niveles más "inteligentes"
    void piensa_nivel_0(Tablero &tab, int &fil, int &col,bool &button) const;


  public:
    //Constructor por defecto.
    Jugador();
    // Cuando construimos un objeto de tipo
    // jugador debemos asignarle un nombre .
    Jugador(const std::string &n);

    //    ~Jugador() { };     // El destructor está vacio
    
    // Necesario para poder asignar objetos de la clase Jugador
    Jugador& operator=(const Jugador &orig);    // Sobrecarga de asignación
    
    // Obtener el nombre del jugador
    std::string Nombre() const         { return nombre; };
    
    // Poner el nombre del jugador
    void setNombre(std::string nom)         { nombre=nom; };
    
       
    // Le damos el tablero y nos devuelve dónde quiere poner ficha el jugador
    void PiensaJugada( Tablero &tab, int &fil, int &col,bool&button) const;
};

/* ***************************************** */

// Para mostrar los datos del jugador en consola
std::ostream& operator<<(std::ostream &salida, const Jugador &jug);

/* ***************************************** */

#endif
