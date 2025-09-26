#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <set>
#include <queue>
#include <bits/stdc++.h>

#include "comportamientos/comportamiento.hpp"

struct estadoA
{
  int fila;
  int columna;
  int orientacion;
  bool zap;

  bool operator == (const estadoA &other) const
  {
    return fila == other.fila && columna == other.columna && orientacion == other.orientacion && zap == other.zap;
  }

  bool operator < (const estadoA &other) const
  {
    if (fila != other.fila) return fila < other.fila;
    if (columna != other.columna) return columna < other.columna;
    if (orientacion != other.orientacion) return orientacion < other.orientacion;
    return zap < other.zap;
  }
};

struct nodoA
{
  estadoA estado;
  list<Action> secuencia;
  float g = 0; // Coste acumulado
  float h = 0; // Heurística

  bool operator == (const nodoA &node) const
  {
    return estado == node.estado && g == node.g && h == node.h;
  }

  bool operator <(const nodoA &node) const
  {
    return (node.g + node.h) < (g + h);
  }
};

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);
  void VisualizaPlan(const estadoA &st, const list<Action> &plan);
  void SituarSensorEnMapa(Sensores sensores);

  Action ComportamientoAuxiliarNivel_E(Sensores sensores);
  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4_Reactivo(Sensores sensores);

  int VeoCasillaInteresanteA(char i, char c, char d, bool zap);
  char ViablePorAlturaA(char casilla, int dif, bool zap);
  list<Action> AnchuraAuxiliar(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> & terreno, const vector<vector<unsigned char>> &altura);
  list<Action> AnchuraAuxiliar_V2(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> & terreno, const vector<vector<unsigned char>> &altura);
  list<Action> AEstrellaAuxiliar(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  bool CasillaAccesibleAuxiliar(const Action &accion, const estadoA &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  estadoA NextCasillaAuxiliar(const estadoA &estado);
  estadoA applyA(Action accion, const estadoA &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  bool Find(const nodoA &nodo, const list<nodoA> &lista);
  int CalcularCoste(Action accion, const estadoA &inicio, estadoA &final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura);
  int CalcularHeuristica(const estadoA &inicio, estadoA &final);
  bool TieneLineaDeVision(const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura, bool zap, Sensores sensores);

private:
  // Definir Variables de Estado
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;

  list<Action> plan;
  bool hayPlan;
};

#endif
