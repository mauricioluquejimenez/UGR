
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
    std::string nombre;   // Nombre del jugador
    Ficha fic;            // Color de la ficha (cruz o circulo)
    int nivel;            // Nivel del jugador
    char tipo;            // Tipo de jugador (L)ocal o (R)emoto
    std::string ip;       // En el caso de jugador remoto la ip.

    // Métodos privados que implementan distintas estrategias de juego
    //   Nivel 0 : Juega una persona
    //   Nivel 1 : Juega la CPU de forma muy básica
    //   ... podríamos implementar nuevos niveles más "inteligentes"
    void piensa_nivel_0(Tablero &tab, int &fil, int &col) const;
    void piensa_nivel_1(const Tablero &tab, int &fil, int &col) const;

  public:
    //Constructor por defecto.
    Jugador();
    // Cuando construimos un objeto de tipo
    // jugador debemos asignarle un nombre y un color obligatoriamente.
    Jugador(const std::string &n, const Ficha &f, int ni, char tip, std::string direc);

    //    ~Jugador() { };     // El destructor está vacio
    
    // Necesario para poder asignar objetos de la clase Jugador
    Jugador& operator=(const Jugador &orig);    // Sobrecarga de asignación
    
    // Obtener el nombre del jugador
    std::string Nombre() const         { return nombre; };
    
    // Poner el nombre del jugador
    void setNombre(std::string nom)         { nombre=nom; };
    
    // Obtener el color de la ficha
    Ficha Color() const                { return fic; };
    
    // Obtener el tipo de jugador (L)ocal (R)emoto
    char gettipo() const                { return tipo; };
    
    //Obtener la direccion ip remota
    std::string getip() const {return ip;};
    
    //Obtener el nivel de Jugador
    int getnivel() const {return nivel;};
    
     //Poner el nivel de Jugador
    void setnivel(int ni) {nivel=ni;};
       
    // Le damos el tablero y nos devuelve dónde quiere poner ficha el jugador
    void PiensaJugada( Tablero &tab, int &fil, int &col) const;
};

/* ***************************************** */

// Para mostrar los datos del jugador en consola
std::ostream& operator<<(std::ostream &salida, const Jugador &jug);

/* ***************************************** */

#endif
