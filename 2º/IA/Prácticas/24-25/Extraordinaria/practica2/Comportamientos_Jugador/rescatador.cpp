#include "../Comportamientos_Jugador/rescatador.hpp"
#include <iostream>
#include "motorlib/util.h"

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	
	// Definición de comportamiento
	if 		(sensores.superficie[0] == 'X') { accion = IDLE;				   }
	else if (sensores.agentes[2] == 'a')	{ giro45izq = 2; accion = TURN_SR; }
	else if (giro45izq != 0) 				{ accion = TURN_SR; giro45izq--;   }
	else
	{
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaInteresanteR(i, c, d, tiene_zapatillas);
		switch (pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
	Action accion;
	
	SituarSensorEnMapa(sensores);

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	
	// Definición de comportamiento
	if 		(sensores.superficie[0] == 'X') { accion = IDLE;				   }
	else if (sensores.agentes[2] == 'a')	{ giro45izq = 2; accion = TURN_SR; }
	else if (giro45izq != 0) 				{ accion = TURN_SR; giro45izq--;   }
	else
	{
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaInteresanteR(i, c, d, tiene_zapatillas);
		switch (pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		estadoR inicio, fin;
		inicio.fila = sensores.posF;
		inicio.columna = sensores.posC;
		inicio.orientacion = sensores.rumbo;
		inicio.zap = tiene_zapatillas;
		fin.fila = sensores.destinoF;
		fin.columna = sensores.destinoC;
		plan = DijkstraRescatador(inicio, fin, mapaResultado, mapaCotas);

		std::cout << "[Nivel_2] Plan generado con " << plan.size() << " acciones." << std::endl;

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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
	Action accion = IDLE;
    static bool esperando = false;
    static int ciclosEspera = 0;
    const int maxCiclosEspera = 10;
    SituarSensorEnMapa(sensores);

    if (sensores.superficie[0] == 'D')tiene_zapatillas = true;

    estadoR inicio, fin;
    inicio.fila = sensores.posF;
    inicio.columna = sensores.posC;
    inicio.orientacion = sensores.rumbo;
    inicio.zap = tiene_zapatillas;
    fin.fila = sensores.destinoF;
    fin.columna = sensores.destinoC;
    // Máquina de estados
    if (esperando)
    {
        ciclosEspera++;
        if (ciclosEspera > maxCiclosEspera)
        {
            esperando = false;
            ciclosEspera = 0;
            accion = CALL_OFF;
        } else accion = IDLE;
    }
    else if (sensores.posF == sensores.destinoF && sensores.posC == sensores.destinoC)
    {
        if (sensores.gravedad)
        {
            esperando = true;
            ciclosEspera = 0;
            accion = CALL_ON;
        } else accion = IDLE;
    }
    else
    {
        if (!hayPlan)
        {
            plan = DijkstraRescatador(inicio, fin, mapaResultado, mapaCotas);
            VisualizaPlan(inicio, plan);
            hayPlan = plan.size() != 0;
        }

        if (hayPlan && plan.size() > 0)
        {
            accion = plan.front();
            plan.pop_front();
            estadoR next = applyR(accion, inicio, mapaResultado, mapaCotas);
            if (!CasillaAccesibleRescatador(accion, next, mapaResultado, mapaCotas))
            {
                plan = DijkstraRescatador(inicio, fin, mapaResultado, mapaCotas);
                VisualizaPlan(inicio, plan);
                hayPlan = plan.size() != 0;
                if (hayPlan && plan.size() > 0)
                {
                    // Si el plan es válido, tomamos la siguiente acción    
                    accion = plan.front();
                    plan.pop_front();
                }
                else accion = ComportamientoRescatadorNivel_4_Reactivo(sensores);

            }
        }
        else if (plan.size() == 0)
        {
            hayPlan = false;
            accion = ComportamientoRescatadorNivel_4_Reactivo(sensores);
        }
    }

    last_action = accion;
    return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4_Reactivo(Sensores sensores)
{
	Action accion;
	SituarSensorEnMapa(sensores);

	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
	if (giro45izq != 0)
	{
		accion = TURN_SR;
		giro45izq--;
	}
	else
	{
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

		int pos = VeoCasillaInteresanteR(i, c, d, tiene_zapatillas);
		switch (pos)
		{
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
		}
	}
	last_action = accion;
	return accion;
}

int ComportamientoRescatador::VeoCasillaInteresanteR(char i, char c, char d, bool zap)
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

char ComportamientoRescatador::ViablePorAlturaR(char casilla, int dif, bool zap)
{
	if(abs(dif) <= 1 or (zap and abs(dif) <= 2)) return casilla;
	else return 'P';
}

list<Action> ComportamientoRescatador::DijkstraRescatador(const estadoR &inicio, const estadoR &final, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	nodoR actual;
	priority_queue<nodoR> abiertos;
	set<estadoR> cerrados;
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
		if (actual.estado.fila == final.fila and actual.estado.columna == final.columna)
		{
			solucion = true;
			break;
		}

		// GENERACIÓN DE HIJO: WALK
		nodoR hijo_WALK = actual;
		hijo_WALK.estado = applyR(WALK, actual.estado, terreno, altura);

		if (!(hijo_WALK.estado == actual.estado) && cerrados.find(hijo_WALK.estado) == cerrados.end())
		{
			hijo_WALK.g = actual.g + CalcularCoste(WALK, actual.estado, hijo_WALK.estado, terreno, altura);
			hijo_WALK.secuencia.push_back(WALK);
			abiertos.push(hijo_WALK);
		}

		// GENERACIÓN DE HIJO: TURN_L
		nodoR hijo_L = actual;
		hijo_L.estado = applyR(TURN_L, actual.estado, terreno, altura);
		if (cerrados.find(hijo_L.estado) == cerrados.end())
		{
			hijo_L.g = actual.g + CalcularCoste(TURN_L, actual.estado, hijo_L.estado, terreno, altura);
			hijo_L.secuencia.push_back(TURN_L);
			abiertos.push(hijo_L);
		}

		// GENERACIÓN DE HIJO: TURN_SR
		nodoR hijo_SR = actual;
		hijo_SR.estado = applyR(TURN_SR, actual.estado, terreno, altura);
		if (cerrados.find(hijo_SR.estado) == cerrados.end())
		{
			hijo_SR.g = actual.g + CalcularCoste(TURN_SR, actual.estado, hijo_SR.estado, terreno, altura);
			hijo_SR.secuencia.push_back(TURN_SR);
			abiertos.push(hijo_SR);
		}
	}

	if (solucion) secuencia = actual.secuencia;

	return secuencia;
}

bool ComportamientoRescatador::CasillaAccesibleRescatador(const Action &accion, const estadoR &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
    if (accion == TURN_L or accion == TURN_SR) return true;

    if (terreno.empty() or terreno[0].empty()) return false;
    if (altura.empty() or altura[0].empty()) return false;

    estadoR intermedio = NextCasillaRescatador(estado);
    estadoR next = NextCasillaRescatador(intermedio);
    estadoR next_walk = intermedio;

    bool check1, check2, check3 = false;

    switch (accion)
    {
        case WALK:
            if (next_walk.fila >= 0 and next_walk.columna >= 0 and next_walk.fila < terreno.size() and next_walk.columna < terreno[0].size())
            {
                if (terreno[next_walk.fila][next_walk.columna] == '?') check1 = check2 = check3 = true;
                else
                {
                    check1 = terreno[next_walk.fila][next_walk.columna] != 'P' and terreno[next_walk.fila][next_walk.columna] != 'M';
                    check2 = terreno[next_walk.fila][next_walk.columna] != 'B';
                    check3 = (abs(altura[next_walk.fila][next_walk.columna] - altura[estado.fila][estado.columna]) <= 1) or (estado.zap and abs(altura[next_walk.fila][next_walk.columna] - altura[estado.fila][estado.columna]) <= 2);
                }
            }
        
            return check1 and check2 and check3;

        case RUN:
            if (next.fila >= 0 and next.columna >= 0 and next.fila < terreno.size() and next.columna < terreno[0].size())
            {
                if (terreno[next.fila][next.columna] == '?') check1 = check2 = check3 = true;
                else
                {
                    check1 = terreno[intermedio.fila][intermedio.columna] != 'P' and terreno[intermedio.fila][intermedio.columna] != 'M' and terreno[intermedio.fila][intermedio.columna] != 'B';
                    check2 = terreno[next.fila][next.columna] != 'P' and terreno[next.fila][next.columna] != 'M' and terreno[next.fila][next.columna] != 'B';
                    check3 = (abs(altura[next.fila][next.columna] - altura[estado.fila][estado.columna]) <= 1) or (estado.zap and abs(altura[next.fila][next.columna] - altura[estado.fila][estado.columna]) <= 2);
                }
            }

            return check1 and check2 and check3;
            
        default:
            return false;
    }
}

estadoR ComportamientoRescatador::NextCasillaRescatador(const estadoR &estado)
{
	estadoR siguiente = estado;

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

estadoR ComportamientoRescatador::applyR(Action accion, const estadoR &estado, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	estadoR siguiente = estado;
	estadoR intermedio;

	switch (accion)
	{
		case WALK:
			if(CasillaAccesibleRescatador(WALK, estado, terreno, altura)) siguiente = NextCasillaRescatador(estado);
			break;

		case RUN:
			if(CasillaAccesibleRescatador(RUN, estado, terreno, altura))
			{
				intermedio = NextCasillaRescatador(estado);
				siguiente = NextCasillaRescatador(intermedio);
			}
			break;
			
		case TURN_SR:
			siguiente.orientacion = (siguiente.orientacion + 1) % 8;
			break;

		case TURN_L:
			siguiente.orientacion = (siguiente.orientacion + 6) % 8;
	}

	return siguiente;
}

int ComportamientoRescatador::CalcularCoste(Action accion, const estadoR &inicio, estadoR &final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura)
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

		case RUN:
			if (casilla == 'A'){ coste = 150; incremento_altura = 15;}
			else if (casilla == 'T') {coste = 35; incremento_altura = 5;}
			else if (casilla == 'S') {coste = 3; incremento_altura = 2;}
			break;

		case TURN_L:
			if (casilla == 'A') coste = 30;
			else if (casilla == 'T') coste = 5;
			else if (casilla == 'S') coste = 1;
			break;
			
		case TURN_SR:
			if (casilla == 'A') coste = 16;
			else if (casilla == 'T') coste = 3;
			else if (casilla == 'S') coste = 1;
			break;
	}

	if (accion == WALK and dif_altura > 0) coste += incremento_altura;
	else if (accion == WALK and dif_altura < 0) coste -= incremento_altura;
	else if (accion == RUN and dif_altura > 0) coste += incremento_altura;
	else if (accion == RUN and dif_altura < 0) coste -= incremento_altura;
	
	return coste;
}

// Nuevo para el Examen Convocatoria Extraordinaria

void AnularMatriz(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

void ComportamientoRescatador::VisualizaPlan(const estadoR &st, const list<Action> &plan)
{
	AnularMatriz(mapaConPlan);
	estadoR cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case RUN:
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
			mapaConPlan[cst.fila][cst.columna] = 3;
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
			mapaConPlan[cst.fila][cst.columna] = 1;
			break;
		case TURN_SR:
			cst.orientacion = (cst.orientacion + 1) % 8;
			break;
		case TURN_L:
			cst.orientacion = (cst.orientacion + 6) % 8;
			break;
		}
		it++;
	}
}

void ComportamientoRescatador::SituarSensorEnMapa(Sensores sensores)
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