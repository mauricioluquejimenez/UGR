#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <set>
#include <queue>  

#include "comportamientos/comportamiento.hpp"

struct estadoR
{
  int fila;
  int columna;
  int orientacion;
  bool zap;

  bool operator == (const estadoR &other) const
  {
    return fila == other.fila && columna == other.columna && orientacion == other.orientacion && zap == other.zap;
  }

  bool operator < (const estadoR &other) const
  {
    if (fila != other.fila) return fila < other.fila;
    if (columna != other.columna) return columna < other.columna;
    if (orientacion != other.orientacion) return orientacion < other.orientacion;
    return zap < other.zap;
  }

};

struct nodoR
{
  estadoR estado;
  list<Action> secuencia;
  float g = 0; // Coste acumulado
  float h = 0; // Heurística

  bool operator == (const nodoR &node) const
  {
    return estado == node.estado && g == node.g;
  }

  bool operator <(const nodoR &node) const
  {
    return node.g < g;
  }
};

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);
  void VisualizaPlan(const estadoR &st, const list<Action> &plan);
  void SituarSensorEnMapa(Sensores sensores);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);
  Action ComportamientoRescatadorNivel_4_Reactivo(Sensores sensores);

  int VeoCasillaInteresanteR(char i, char c, char d, bool zap);
  char ViablePorAlturaR(char casilla, int dif, bool zap);
  list<Action> DijkstraRescatador(const estadoR &inicio, const estadoR &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  
  bool CasillaAccesibleRescatador(const Action &accion, const estadoR &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  estadoR NextCasillaRescatador(const estadoR &estado);
  estadoR applyR(Action accion, const estadoR &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  int CalcularCoste(Action accion, const estadoR &inicio, estadoR &final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura);

private:
  // Variables de Estado
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;

  list<Action> plan;
  bool hayPlan;
};

#endif
