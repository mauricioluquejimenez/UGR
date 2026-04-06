#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3 (sensores);
		//accion = ComportamientoAuxiliarNivel_E(sensores);
		break;
	case 4:
		accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		estadoA inicio, fin;
		inicio.fila = sensores.posF;
		inicio.columna = sensores.posC;
		inicio.orientacion = sensores.rumbo;
		inicio.zap = tiene_zapatillas;
		fin.fila = sensores.destinoF;
		fin.columna = sensores.destinoC;
		plan = AnchuraAuxiliar_V2(inicio, fin, mapaResultado, mapaCotas);

		std::cout << "[Nivel_E] Plan generado con " << plan.size() << " acciones." << std::endl;

		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0)
	{
		hayPlan = false;
	}
	
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	
	// Definición de comportamiento
	if 		(sensores.superficie[0] == 'X') { accion = IDLE;				 }
	else if (giro45izq > 0)					{ accion = TURN_SR; giro45izq--; }
	else if (giro45izq < 0)					{ accion = TURN_SR; giro45izq++; }
	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaInteresanteA(i, c, d, tiene_zapatillas);
		switch (pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = -6;
				accion = TURN_SR;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				giro45izq = -6;
				accion = TURN_SR;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;

	SituarSensorEnMapa(sensores);

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	
	// Definición de comportamiento
	if 		(sensores.superficie[0] == 'X') { accion = IDLE;				 }
	else if (giro45izq > 0)					{ accion = TURN_SR; giro45izq--; }
	else if (giro45izq < 0)					{ accion = TURN_SR; giro45izq++; }
	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaInteresanteA(i, c, d, tiene_zapatillas);
		switch (pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = -6;
				accion = TURN_SR;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				giro45izq = -6;
				accion = TURN_SR;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		estadoA inicio, fin;
		inicio.fila = sensores.posF;
		inicio.columna = sensores.posC;
		inicio.orientacion = sensores.rumbo;
		inicio.zap = tiene_zapatillas;
		fin.fila = sensores.destinoF;
		fin.columna = sensores.destinoC;
		plan = AEstrellaAuxiliar(inicio, fin, mapaResultado, mapaCotas);

		std::cout << "[Nivel_3] Plan generado con " << plan.size() << " acciones." << std::endl;

		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if (hayPlan and plan.size() > 0)
	{
		accion = plan.front();
		plan.pop_front();
	}
	if (plan.size() == 0)
	{
		hayPlan = false;
	}
	
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
	Action accion = IDLE;
	static bool enMisionRescate = false;
	static estadoA destinoRescate;
	static int ciclosEspera = 0;
	const int maxCiclosEspera = 20;
	SituarSensorEnMapa(sensores);

	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;

	estadoA inicio;
	inicio.fila = sensores.posF;
	inicio.columna = sensores.posC;
	inicio.orientacion = sensores.rumbo;
	inicio.zap = tiene_zapatillas;

	// Comprobar si tenemos una misión de rescate
	if (sensores.destinoF != -1 && sensores.destinoC != -1)
	{
		enMisionRescate = true;
		destinoRescate.fila = sensores.destinoF;
		destinoRescate.columna = sensores.destinoC;
		ciclosEspera = 0;
	}
	if (enMisionRescate) {
		if (TieneLineaDeVision(mapaResultado, mapaCotas, tiene_zapatillas, sensores))
		{
			enMisionRescate = false;
			accion = IDLE;
			hayPlan = false;
			plan.clear();
		}
		else
		{
			if (!hayPlan)
			{
				plan = AEstrellaAuxiliar(inicio, destinoRescate, mapaResultado, mapaCotas);
				VisualizaPlan(inicio, plan);
				hayPlan = plan.size() != 0;
				ciclosEspera = 0;
			}

			if (hayPlan && plan.size() > 0)
			{
				accion = plan.front();
				plan.pop_front();

				estadoA next = applyA(accion, inicio, mapaResultado, mapaCotas);

				if (!CasillaAccesibleAuxiliar(accion, next, mapaResultado, mapaCotas))
				{
					plan = AEstrellaAuxiliar(inicio, destinoRescate, mapaResultado, mapaCotas);
					VisualizaPlan(inicio, plan);
					hayPlan = plan.size() != 0;
					ciclosEspera++;

					if (hayPlan && plan.size() > 0)
					{
						accion = plan.front();
						plan.pop_front();
					}
					else accion = ComportamientoAuxiliarNivel_4_Reactivo(sensores);
		
				}
			}
			else if (plan.size() == 0)
			{
				hayPlan = false;
				accion = ComportamientoAuxiliarNivel_4_Reactivo(sensores);
			}
				
			if (ciclosEspera > maxCiclosEspera)
			{
				enMisionRescate = false;
				hayPlan = false;
				accion = IDLE;
				plan.clear();
			}
		}
	}
	else
	{
		// Comportamiento de exploración
		if (giro45izq != 0)
		{
			accion = TURN_SR;
			giro45izq--;
		}
		else
		{
			char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
			char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
			char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
			int pos = VeoCasillaInteresanteA(i, c, d, tiene_zapatillas);

			switch (pos)
			{
				case 2: accion = WALK; break;
				case 1: giro45izq = 6; accion = TURN_SR; break;
				case 3: accion = TURN_SR; break;
				default: accion = TURN_SR; break;
		}
		}
	}
	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4_Reactivo(Sensores sensores)
{
	Action accion;

	// Priorizar recoger zapatillas si no las tenemos
	if (!tiene_zapatillas)
	{
		for (int i = 1; i <= 3; i++)
		{
			if (sensores.superficie[i] == 'D')
			{
				char viable = ViablePorAlturaA(sensores.superficie[i], sensores.cota[i] - sensores.cota[0], tiene_zapatillas);
				
				if (viable != 'P')
				{
					if (i == 1) { giro45izq = 6; return TURN_SR; }
					if (i == 2) return WALK;
					if (i == 3) return TURN_SR;
				}
			}
		}
	}
	
	// Comportamiento reactivo normal
	return ComportamientoAuxiliarNivel_4_Reactivo(sensores);
}

int ComportamientoAuxiliar::VeoCasillaInteresanteA(char i, char c, char d, bool zap)
{
	if (c == 'X') return 2;
	else if (i == 'X') return 1;
	else if (d == 'X') return 3;
	else if (!zap)
	{
		if (c == 'D') return 2;
		else if (i == 'D') return 1;
		else if (d == 'D') return 3;
	}
	if (c == 'C') return 2;
	else if (i == 'C') return 1;
	else if (d == 'C') return 3;
	else return 0;
}

char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif, bool zap)
{
	if(abs(dif) <= 1) return casilla;
	else return 'P';
}

list<Action> AvanzaASaltosDeCaballos()
{
	list<Action> secuencia;

	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);

	return secuencia;
}

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	nodoA actual;
	list<nodoA> abiertos;
	list<nodoA> cerrados;
	list<Action> secuencia;

	actual.estado = inicio;
	abiertos.push_back(actual);
	bool solucion = (actual.estado.fila == final.fila and actual.estado.columna == final.columna);
	while (!solucion and !abiertos.empty())
	{
		abiertos.pop_front();
		cerrados.push_back(actual);

		if (terreno[actual.estado.fila][actual.estado.columna] == 'D') actual.estado.zap = true;

		nodoA hijo_WALK = actual;
		hijo_WALK.estado = applyA(WALK, actual.estado, terreno, altura);
		if (hijo_WALK.estado.fila == final.fila and hijo_WALK.estado.columna == final.columna)
		{
			hijo_WALK.secuencia.push_back(WALK);
			actual = hijo_WALK;
			solucion = true;
		}
		else if (!Find(hijo_WALK, abiertos) and !Find(hijo_WALK, cerrados))
		{
			hijo_WALK.secuencia.push_back(WALK);
			abiertos.push_back(hijo_WALK);
		}

		if (!solucion)
		{
			nodoA hijo_TURN_SR = actual;
			hijo_TURN_SR.estado = applyA(TURN_SR, actual.estado, terreno, altura);
			if (!Find(hijo_TURN_SR, abiertos) and !Find(hijo_TURN_SR, cerrados))
			{
				hijo_TURN_SR.secuencia.push_back(TURN_SR);
				abiertos.push_back(hijo_TURN_SR);
			}
		}

		if (!solucion and !abiertos.empty())
		{
			actual = abiertos.front();
			solucion = (actual.estado == final);
		}
	}

	if (solucion) secuencia = actual.secuencia;

	return secuencia;
}

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar_V2(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	nodoA actual;
	list<nodoA> abiertos;
	set<nodoA> cerrados;
	list<Action> secuencia;

	actual.estado = inicio;
	abiertos.push_back(actual);
	bool solucion = (actual.estado.fila == final.fila and actual.estado.columna == final.columna);
	while (!solucion and !abiertos.empty())
	{
		abiertos.pop_front();
		cerrados.insert(actual);

		if (terreno[actual.estado.fila][actual.estado.columna] == 'D') actual.estado.zap = true;

		nodoA hijo_WALK = actual;
		hijo_WALK.estado = applyA(WALK, actual.estado, terreno, altura);
		if (hijo_WALK.estado.fila == final.fila and hijo_WALK.estado.columna == final.columna)
		{
			hijo_WALK.secuencia.push_back(WALK);
			actual = hijo_WALK;
			solucion = true;
		}
		else if (cerrados.find(hijo_WALK) == cerrados.end())
		{
			hijo_WALK.secuencia.push_back(WALK);
			abiertos.push_back(hijo_WALK);
		}

		if (!solucion)
		{
			nodoA hijo_TURN_SR = actual;
			hijo_TURN_SR.estado = applyA(TURN_SR, actual.estado, terreno, altura);
			if (cerrados.find(hijo_TURN_SR) == cerrados.end())
			{	
				hijo_TURN_SR.secuencia.push_back(TURN_SR);
				abiertos.push_back(hijo_TURN_SR);
			}
		}

		if (!solucion and !abiertos.empty())
		{
			actual = abiertos.front();
			while (cerrados.find(actual) != cerrados.end() and !abiertos.empty()) 
			{
				abiertos.pop_front();
				actual = abiertos.front();
			}
		}
	}

	if (solucion) secuencia = actual.secuencia;

	return secuencia;
}

list<Action> ComportamientoAuxiliar::AEstrellaAuxiliar(const estadoA &inicio, const estadoA &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	nodoA actual;
	estadoA nodo_final = final;
	priority_queue<nodoA> abiertos;
	set<estadoA> cerrados;
	list<Action> secuencia;

	actual.estado = inicio;
	abiertos.push(actual);
	bool solucion = (actual.estado.fila == final.fila and actual.estado.columna == final.columna);

	while (!solucion and !abiertos.empty())
	{
		actual = abiertos.top();
		abiertos.pop();

		// Si ya se ha explorado este estado, lo ignoramos
		if (cerrados.find(actual.estado) != cerrados.end()) continue;

		// Marcar como explorado
		cerrados.insert(actual.estado);

		// Recoger zapatillas si está en una casilla 'D'
		if (terreno[actual.estado.fila][actual.estado.columna] == 'D') actual.estado.zap = true;

		// Comprobación de objetivo
		if (actual.estado.fila == final.fila && actual.estado.columna == final.columna) {
			solucion = true;
			break;
		}

		// GENERACIÓN DE HIJO: WALK
		nodoA hijo_WALK = actual;
		hijo_WALK.estado = applyA(WALK, actual.estado, terreno, altura);

		if (!(hijo_WALK.estado == actual.estado) && cerrados.find(hijo_WALK.estado) == cerrados.end())
		{
			hijo_WALK.g = actual.g + CalcularCoste(WALK, actual.estado, hijo_WALK.estado, terreno, altura);
			hijo_WALK.h = CalcularHeuristica(hijo_WALK.estado, nodo_final);
			hijo_WALK.secuencia.push_back(WALK);
			abiertos.push(hijo_WALK);
		}

		// GENERACIÓN DE HIJO: TURN_SR
		nodoA hijo_SR = actual;
		hijo_SR.estado = applyA(TURN_SR, actual.estado, terreno, altura);
		if (cerrados.find(hijo_SR.estado) == cerrados.end())
		{
			hijo_SR.g = actual.g + CalcularCoste(TURN_SR, actual.estado, hijo_SR.estado, terreno, altura);
			hijo_SR.h = CalcularHeuristica(hijo_SR.estado, nodo_final);
			hijo_SR.secuencia.push_back(TURN_SR);
			abiertos.push(hijo_SR);
		}
	}

	if (solucion) secuencia = actual.secuencia;

	return secuencia;
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const Action &accion, const estadoA &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
    if (accion == TURN_L || accion == TURN_SR) return true;

    if (terreno.empty() || terreno[0].empty()) return false;
    if (altura.empty() || altura[0].empty()) return false;

    estadoA intermedio = NextCasillaAuxiliar(estado);
    estadoA next = NextCasillaAuxiliar(intermedio);
    estadoA next_walk = intermedio;

    bool check1 = false, check2 = false, check3 = false;

    switch (accion)
    {
        case WALK:
            if (next_walk.fila >= 0 && next_walk.columna >= 0 && next_walk.fila < terreno.size() && next_walk.columna < terreno[0].size())
            {
                if (terreno[next_walk.fila][next_walk.columna] == '?') check1 = check2 = check3 = true;
                else
                {
                    check1 = terreno[next_walk.fila][next_walk.columna] != 'P' && terreno[next_walk.fila][next_walk.columna] != 'M';
                    check2 = terreno[next_walk.fila][next_walk.columna] != 'B';
                    check3 = (abs(altura[next_walk.fila][next_walk.columna] - altura[estado.fila][estado.columna]) <= 1) || (estado.zap && abs(altura[next_walk.fila][next_walk.columna] - altura[estado.fila][estado.columna]) <= 2);
                }
            }
        
            return check1 && check2 && check3;

        case RUN:
            if (next.fila >= 0 && next.columna >= 0 && next.fila < terreno.size() && next.columna < terreno[0].size())
            {
                if (terreno[next.fila][next.columna] == '?') check1 = check2 = check3 = true;
                else
                {
                    check1 = terreno[intermedio.fila][intermedio.columna] != 'P' && terreno[intermedio.fila][intermedio.columna] != 'M' && terreno[intermedio.fila][intermedio.columna] != 'B';
                    check2 = terreno[next.fila][next.columna] != 'P' && terreno[next.fila][next.columna] != 'M' && terreno[next.fila][next.columna] != 'B';
                    check3 = (abs(altura[next.fila][next.columna] - altura[estado.fila][estado.columna]) <= 1) || (estado.zap && abs(altura[next.fila][next.columna] - altura[estado.fila][estado.columna]) <= 2);
                }
            }

            return check1 && check2 && check3;
            
        default:
            return false;
    }
}

estadoA ComportamientoAuxiliar::NextCasillaAuxiliar(const estadoA &estado)
{
	estadoA siguiente = estado;

	switch(estado.orientacion)
	{
		case norte:
			siguiente.fila = estado.fila - 1;
			break;

		case noreste:
			siguiente.fila = estado.fila - 1;
			siguiente.columna = estado.columna + 1;
			break;

		case este:
			siguiente.columna = estado.columna + 1;
			break;

		case sureste:
			siguiente.fila = estado.fila + 1;
			siguiente.columna = estado.columna + 1;
		break;

		case sur:
			siguiente.fila = estado.fila + 1;
			break;

		case suroeste:
			siguiente.fila = estado.fila + 1;
			siguiente.columna = estado.columna - 1;
			break;

		case oeste:
			siguiente.columna = estado.columna - 1;
			break;

		case noroeste:
			siguiente.fila = estado.fila - 1;
			siguiente.columna = estado.columna - 1;
			break;
	}

	return siguiente;
}

estadoA ComportamientoAuxiliar::applyA(Action accion, const estadoA &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	estadoA siguiente = estado;
	estadoA intermedio;

	switch (accion)
	{
		case WALK:
			if(CasillaAccesibleAuxiliar(WALK, estado, terreno, altura)) siguiente = NextCasillaAuxiliar(estado);
			break;
			
		case TURN_SR:
			siguiente.orientacion = (siguiente.orientacion + 1) % 8;
			break;
	}

	return siguiente;
}

bool ComportamientoAuxiliar::Find(const nodoA &nodo, const list<nodoA> &lista)
  {
    auto it = lista.begin();
    
    while (it != lista.end() and !((*it) == nodo)) it++;
    
    return (it != lista.end());
  }

  int ComportamientoAuxiliar::CalcularCoste(Action accion, const estadoA &inicio, estadoA &final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura)
  {
	char casilla = terreno[inicio.fila][inicio.columna];
	int dif_altura = altura[final.fila][final.columna] - altura[inicio.fila][inicio.columna];
	int coste = 1, incremento_altura = 0;

	switch(accion)
	{
		case WALK:
			if (casilla == 'A'){ coste = 100; incremento_altura = 10;}
			else if (casilla == 'T') {coste = 20; incremento_altura = 5;}
			else if (casilla == 'S') {coste = 2; incremento_altura = 1;}
			break;
			
		case TURN_SR:
			if (casilla == 'A') coste = 16;
			else if (casilla == 'T') coste = 3;
			else if (casilla == 'S') coste = 1;
			break;
	}

	if (accion == WALK && dif_altura > 0) coste += incremento_altura;
	else if (accion == WALK && dif_altura < 0) coste -= incremento_altura;
	else if (accion == RUN && dif_altura > 0) coste += incremento_altura;
	else if (accion == RUN && dif_altura < 0) coste -= incremento_altura;
	
	return coste;
  }

  int ComportamientoAuxiliar::CalcularHeuristica(const estadoA &inicio, estadoA &final)
  {
	int a = abs(inicio.fila - final.fila);
	int b = abs(inicio.columna - final.columna);
	return (a > b) ? a : b;
  }

  bool ComportamientoAuxiliar::TieneLineaDeVision(const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura, bool zap, Sensores sensores)
  {
	if (sensores.posF < 0 || sensores.posF >= terreno.size() || sensores.posC < 0 || sensores.posC >= terreno[0].size() || sensores.destinoF < 0 || sensores.destinoF >= terreno.size() || sensores.destinoC < 0 || sensores.destinoC >= terreno[0].size()) return false;

	int deltaF = abs(sensores.destinoF - sensores.posF);
	int deltaC = abs(sensores.destinoC - sensores.posC);
	int stepF = (sensores.posF < sensores.destinoF) ? 1 : -1;
	int stepC = (sensores.posC < sensores.destinoC) ? 1 : -1;

	int currentF = sensores.posF;
	int currentC = sensores.posC;
	int currentAltura = altura[currentF][currentC];

	while (currentF != sensores.destinoF || currentC != sensores.destinoC)
	{
		if (currentF != sensores.destinoF) currentF += stepF;
		if (currentC != sensores.destinoC) currentC += stepC;

		if (currentF < 0 || currentF >= terreno.size() || currentC < 0 || currentC >= terreno[0].size()) return false;

		// Verificar obstáculos
		if (terreno[currentF][currentC] == 'P' || terreno[currentF][currentC] == 'M') return false;

		// Verificar desnivel
		int desnivel = abs(altura[currentF][currentC] - currentAltura);

		if ( desnivel > 1) return false;

		currentAltura = altura[currentF][currentC];
	}
	
	return true;
}

// Nuevo para el Examen Convocatoria Extraordinaria

void AnularMatrizAux(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

void ComportamientoAuxiliar::VisualizaPlan(const estadoA &st, const list<Action> &plan)
{
	AnularMatrizAux(mapaConPlan);
	estadoA cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
			case WALK:
				switch (cst.orientacion)
				{
					case 0:
						cst.fila--;
						break;
					
					case 1:
						cst.fila--;
						cst.columna++;
						break;
					
					case 2:
						cst.columna++;
						break;
					
					case 3:
						cst.fila++;
						cst.columna++;
						break;
					
					case 4:
						cst.fila++;
						break;
					
					case 5:
						cst.fila++;
						cst.columna--;
						break;
					
					case 6:
						cst.columna--;
						break;
					
					case 7:
						cst.fila--;
						cst.columna--;
						break;
				}
				mapaConPlan[cst.fila][cst.columna] = 2;
			break;
			
			case TURN_SR:
				cst.orientacion = (cst.orientacion + 1) % 8;
			break;
		}
		it++;
	}
}

void ComportamientoAuxiliar::SituarSensorEnMapa(Sensores sensores)
{
	mapaResultado[sensores.posF][sensores.posC] = sensores.superficie[0];
	mapaCotas[sensores.posF][sensores.posC] = sensores.cota[0];

	int pos = 1;
	switch (sensores.rumbo)
	{
	case 0:
		for (int j = 1; j < 4; j++)
		{
			for (int i = -j; i <= j; i++)
			{
				mapaResultado[sensores.posF - j][sensores.posC + i] = sensores.superficie[pos];
				mapaCotas[sensores.posF - j][sensores.posC + i] = sensores.cota[pos];
				pos++;
			}
		}
		break;
	case 1:
		mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[1];
		mapaResultado[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
		mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[3];
		mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[4];
		mapaResultado[sensores.posF - 2][sensores.posC + 1] = sensores.superficie[5];
		mapaResultado[sensores.posF - 2][sensores.posC + 2] = sensores.superficie[6];
		mapaResultado[sensores.posF - 1][sensores.posC + 2] = sensores.superficie[7];
		mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[8];
		mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[9];
		mapaResultado[sensores.posF - 3][sensores.posC + 1] = sensores.superficie[10];
		mapaResultado[sensores.posF - 3][sensores.posC + 2] = sensores.superficie[11];
		mapaResultado[sensores.posF - 3][sensores.posC + 3] = sensores.superficie[12];
		mapaResultado[sensores.posF - 2][sensores.posC + 3] = sensores.superficie[13];
		mapaResultado[sensores.posF - 1][sensores.posC + 3] = sensores.superficie[14];
		mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[15];
		mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[1];
		mapaCotas[sensores.posF - 1][sensores.posC + 1] = sensores.cota[2];
		mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[3];
		mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[4];
		mapaCotas[sensores.posF - 2][sensores.posC + 1] = sensores.cota[5];
		mapaCotas[sensores.posF - 2][sensores.posC + 2] = sensores.cota[6];
		mapaCotas[sensores.posF - 1][sensores.posC + 2] = sensores.cota[7];
		mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[8];
		mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[9];
		mapaCotas[sensores.posF - 3][sensores.posC + 1] = sensores.cota[10];
		mapaCotas[sensores.posF - 3][sensores.posC + 2] = sensores.cota[11];
		mapaCotas[sensores.posF - 3][sensores.posC + 3] = sensores.cota[12];
		mapaCotas[sensores.posF - 2][sensores.posC + 3] = sensores.cota[13];
		mapaCotas[sensores.posF - 1][sensores.posC + 3] = sensores.cota[14];
		mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[15];
		break;
	case 2:
		for (int j = 1; j < 4; j++)
		{
			for (int i = -j; i <= j; i++)
			{
				mapaResultado[sensores.posF + i][sensores.posC + j] = sensores.superficie[pos];
				mapaCotas[sensores.posF + i][sensores.posC + j] = sensores.cota[pos];
				pos++;
			}
		}
		break;
	case 3:
		mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[1];
		mapaResultado[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
		mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[3];
		mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[4];
		mapaResultado[sensores.posF + 1][sensores.posC + 2] = sensores.superficie[5];
		mapaResultado[sensores.posF + 2][sensores.posC + 2] = sensores.superficie[6];
		mapaResultado[sensores.posF + 2][sensores.posC + 1] = sensores.superficie[7];
		mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[8];
		mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[9];
		mapaResultado[sensores.posF + 1][sensores.posC + 3] = sensores.superficie[10];
		mapaResultado[sensores.posF + 2][sensores.posC + 3] = sensores.superficie[11];
		mapaResultado[sensores.posF + 3][sensores.posC + 3] = sensores.superficie[12];
		mapaResultado[sensores.posF + 3][sensores.posC + 2] = sensores.superficie[13];
		mapaResultado[sensores.posF + 3][sensores.posC + 1] = sensores.superficie[14];
		mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[15];

		mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[1];
		mapaCotas[sensores.posF + 1][sensores.posC + 1] = sensores.cota[2];
		mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[3];
		mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[4];
		mapaCotas[sensores.posF + 1][sensores.posC + 2] = sensores.cota[5];
		mapaCotas[sensores.posF + 2][sensores.posC + 2] = sensores.cota[6];
		mapaCotas[sensores.posF + 2][sensores.posC + 1] = sensores.cota[7];
		mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[8];
		mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[9];
		mapaCotas[sensores.posF + 1][sensores.posC + 3] = sensores.cota[10];
		mapaCotas[sensores.posF + 2][sensores.posC + 3] = sensores.cota[11];
		mapaCotas[sensores.posF + 3][sensores.posC + 3] = sensores.cota[12];
		mapaCotas[sensores.posF + 3][sensores.posC + 2] = sensores.cota[13];
		mapaCotas[sensores.posF + 3][sensores.posC + 1] = sensores.cota[14];
		mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[15];
		break;
	case 4:
		for (int j = 1; j < 4; j++)
		{
			for (int i = -j; i <= j; i++)
			{
				mapaResultado[sensores.posF + j][sensores.posC - i] = sensores.superficie[pos];
				mapaCotas[sensores.posF + j][sensores.posC - i] = sensores.cota[pos];
				pos++;
			}
		}
		break;
	case 5:
		mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[1];
		mapaResultado[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
		mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[3];
		mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[4];
		mapaResultado[sensores.posF + 2][sensores.posC - 1] = sensores.superficie[5];
		mapaResultado[sensores.posF + 2][sensores.posC - 2] = sensores.superficie[6];
		mapaResultado[sensores.posF + 1][sensores.posC - 2] = sensores.superficie[7];
		mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[8];
		mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[9];
		mapaResultado[sensores.posF + 3][sensores.posC - 1] = sensores.superficie[10];
		mapaResultado[sensores.posF + 3][sensores.posC - 2] = sensores.superficie[11];
		mapaResultado[sensores.posF + 3][sensores.posC - 3] = sensores.superficie[12];
		mapaResultado[sensores.posF + 2][sensores.posC - 3] = sensores.superficie[13];
		mapaResultado[sensores.posF + 1][sensores.posC - 3] = sensores.superficie[14];
		mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[15];

		mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[1];
		mapaCotas[sensores.posF + 1][sensores.posC - 1] = sensores.cota[2];
		mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[3];
		mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[4];
		mapaCotas[sensores.posF + 2][sensores.posC - 1] = sensores.cota[5];
		mapaCotas[sensores.posF + 2][sensores.posC - 2] = sensores.cota[6];
		mapaCotas[sensores.posF + 1][sensores.posC - 2] = sensores.cota[7];
		mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[8];
		mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[9];
		mapaCotas[sensores.posF + 3][sensores.posC - 1] = sensores.cota[10];
		mapaCotas[sensores.posF + 3][sensores.posC - 2] = sensores.cota[11];
		mapaCotas[sensores.posF + 3][sensores.posC - 3] = sensores.cota[12];
		mapaCotas[sensores.posF + 2][sensores.posC - 3] = sensores.cota[13];
		mapaCotas[sensores.posF + 1][sensores.posC - 3] = sensores.cota[14];
		mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[15];
		break;

	case 6:
		for (int j = 1; j < 4; j++)
		{
			for (int i = -j; i <= j; i++)
			{
				mapaResultado[sensores.posF - i][sensores.posC - j] = sensores.superficie[pos];
				mapaCotas[sensores.posF - i][sensores.posC - j] = sensores.cota[pos];
				pos++;
			}
		}
		break;
	case 7:
		mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[1];
		mapaResultado[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
		mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[3];
		mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[4];
		mapaResultado[sensores.posF - 1][sensores.posC - 2] = sensores.superficie[5];
		mapaResultado[sensores.posF - 2][sensores.posC - 2] = sensores.superficie[6];
		mapaResultado[sensores.posF - 2][sensores.posC - 1] = sensores.superficie[7];
		mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[8];
		mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[9];
		mapaResultado[sensores.posF - 1][sensores.posC - 3] = sensores.superficie[10];
		mapaResultado[sensores.posF - 2][sensores.posC - 3] = sensores.superficie[11];
		mapaResultado[sensores.posF - 3][sensores.posC - 3] = sensores.superficie[12];
		mapaResultado[sensores.posF - 3][sensores.posC - 2] = sensores.superficie[13];
		mapaResultado[sensores.posF - 3][sensores.posC - 1] = sensores.superficie[14];
		mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[15];

		mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[1];
		mapaCotas[sensores.posF - 1][sensores.posC - 1] = sensores.cota[2];
		mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[3];
		mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[4];
		mapaCotas[sensores.posF - 1][sensores.posC - 2] = sensores.cota[5];
		mapaCotas[sensores.posF - 2][sensores.posC - 2] = sensores.cota[6];
		mapaCotas[sensores.posF - 2][sensores.posC - 1] = sensores.cota[7];
		mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[8];
		mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[9];
		mapaCotas[sensores.posF - 1][sensores.posC - 3] = sensores.cota[10];
		mapaCotas[sensores.posF - 2][sensores.posC - 3] = sensores.cota[11];
		mapaCotas[sensores.posF - 3][sensores.posC - 3] = sensores.cota[12];
		mapaCotas[sensores.posF - 3][sensores.posC - 2] = sensores.cota[13];
		mapaCotas[sensores.posF - 3][sensores.posC - 1] = sensores.cota[14];
		mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[15];
		break;
	}
}