
#ifndef __TABLERO__H__
#define __TABLERO__H__

/* ***************************************** */

#include <iostream>
#include "ficha.h"
#include "tablerografico.h"
#include <map>
/* ***************************************** */

class Tablero {

  private:
    Ficha tab[3][3];      // Tablero de fichas 3x3
    int numfichas;        // Número de fichas que han sido puestas en total
    TableroGrafico *TG;   //puntero al tablero grafico
    std::map<Ficha,std::string> imagen_ficha;// mapa con los nombre de las imagenes de las fichas
    // Copia un tablero desde orig
    void copia_tablero(const Tablero &orig);

    // Devuelve true/false si la ficha de tipo fic tiene 3 en raya
    bool hay3raya(const Ficha &fic) const;


  public:
    Tablero();                                  // Constructor por defecto
                                     // Constructor por defecto
    ~Tablero() { };                             // Destructor (vacío)

    Tablero(const Tablero &orig);               // Constructor de copia
    Tablero& operator=(const Tablero &orig);    // Sobrecarga de asignación

    // Inicializa el tablero poniendo en blanco todas sus casillas
    void PonerEnBlanco();

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

    //Obtiene la casilla pulsada. True si ha conseguido una fila y columna. False si se ha cerrado con X
    void getCasillaElegida(int &fil,int &col){
        TG->getCasillaElegida(fil,col);
    }
    void setTableroGrafico(TableroGrafico *tg){
        TG=tg;
    }
    //Asigna los nombres de los ficheros  a cada ficha
    void setFilesFichas(std::string nameimg[], Ficha * fichas,int nfichas);

    //Dibuja el tablero grafico entero
    void Redraw();


};

// Para mostrar el tablero en pantalla sobrecargamos <<
std::ostream& operator<<(std::ostream &salida, const Tablero &tab);

/* ***************************************** */

#endif
