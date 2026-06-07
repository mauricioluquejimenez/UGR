#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>

#include "comportamientos/comportamiento.hpp"

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    hayPlan = false;
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  int VeoCasillaInteresanteA(char i, char c, char d);
  int VeoCasillaInteresanteA_1(char i, char c, char d);
  char ViablePorAlturaA(char casilla, int dif);
  void SituarSensorEnMapaA(std::vector<std::vector<unsigned char>> &m, std::vector<std::vector<unsigned char>> &a, Sensores sensores);
  list<Action> ComportamientoAuxiliar::AvanzaASaltosDeCaballo();
  
  Action ComportamientoAuxiliarNivelE(Sensores sensores);
  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);

private:
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;
  list<Action> plan;
  bool hayPlan;
};

#endif
