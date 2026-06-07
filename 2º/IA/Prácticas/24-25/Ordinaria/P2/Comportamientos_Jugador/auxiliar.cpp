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
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		accion = ComportamientoAuxiliarNivelE (sensores);
		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

int ComportamientoAuxiliar::VeoCasillaInteresanteA(char i, char c, char d)
{
	if (c == 'X') return 2;
	else if (i == 'X') return 1;
	else if (d == 'X') return 3;
	else if (c == 'C') return 2;
	else if (i == 'C') return 1;
	else if (d == 'C') return 3;
	else return 0;
}

int ComportamientoAuxiliar::VeoCasillaInteresanteA_1(char i, char c, char d)
    {
        if 		(c == 'X') 			   return 2;
		else if (i == 'X') 			   return 1;
		else if (d == 'X')			   return 3;
		else if (c == 'C' or c == 'S') return 2;
		else if (i == 'C' or c == 'S') return 1;
		else if (d == 'C' or c == 'S') return 3;
    }

char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif)
{
	if(abs(dif) <= 1) return casilla;
	else return 'P';
}

void ComportamientoAuxiliar::SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
{
	// Casilla central
	m[sensores.posF][sensores.posC] = sensores.superficie[0];
	a[sensores.posF][sensores.posC] = sensores.cota[0];

	int pos = 1;
	switch (sensores.rumbo)
	{
		case norte:
			m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[1];
			a[sensores.posF - 1][sensores.posC - 1] = sensores.cota[1];

			m[sensores.posF - 1][sensores.posC	  ] = sensores.superficie[2];
			a[sensores.posF - 1][sensores.posC	  ] = sensores.cota[2];

			m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[3];
			a[sensores.posF - 1][sensores.posC + 1] = sensores.cota[3];
			break;

		case noreste:
			m[sensores.posF - 1][sensores.posC	  ] = sensores.superficie[1];
			a[sensores.posF - 1][sensores.posC	  ] = sensores.cota[1];

			m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
			a[sensores.posF - 1][sensores.posC + 1] = sensores.cota[2];

			m[sensores.posF	   ][sensores.posC + 1] = sensores.superficie[3];
			a[sensores.posF	   ][sensores.posC + 1] = sensores.cota[3];
			break;

		case este:
			m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[1];
			a[sensores.posF - 1][sensores.posC + 1] = sensores.cota[1];

			m[sensores.posF	   ][sensores.posC + 1] = sensores.superficie[2];
			a[sensores.posF	   ][sensores.posC + 1] = sensores.cota[2];

			m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[3];
			a[sensores.posF + 1][sensores.posC + 1] = sensores.cota[3];
			break;

		case sureste:
			m[sensores.posF + 1][sensores.posC	  ] = sensores.superficie[1];
			a[sensores.posF + 1][sensores.posC	  ] = sensores.cota[1];

			m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
			a[sensores.posF + 1][sensores.posC + 1] = sensores.cota[2];

			m[sensores.posF	   ][sensores.posC + 1] = sensores.superficie[3];
			a[sensores.posF	   ][sensores.posC + 1] = sensores.cota[3];
			break;

		case sur:
			m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[1];
			a[sensores.posF + 1][sensores.posC - 1] = sensores.cota[1];

			m[sensores.posF + 1][sensores.posC	  ] = sensores.superficie[2];
			a[sensores.posF + 1][sensores.posC	  ] = sensores.cota[2];

			m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[3];
			a[sensores.posF + 1][sensores.posC + 1] = sensores.cota[3];
			break;

		case suroeste:
			m[sensores.posF + 1][sensores.posC	  ] = sensores.superficie[1];
			a[sensores.posF + 1][sensores.posC	  ] = sensores.cota[1];

			m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
			a[sensores.posF + 1][sensores.posC - 1] = sensores.cota[2];

			m[sensores.posF	   ][sensores.posC - 1] = sensores.superficie[3];
			a[sensores.posF	   ][sensores.posC - 1] = sensores.cota[3];
			break;

		case oeste:
			m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[1];
			a[sensores.posF - 1][sensores.posC - 1] = sensores.cota[1];

			m[sensores.posF	   ][sensores.posC - 1] = sensores.superficie[2];
			a[sensores.posF	   ][sensores.posC - 1] = sensores.cota[2];

			m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[3];
			a[sensores.posF + 1][sensores.posC - 1] = sensores.cota[3];
			break;

		case noroeste:
			m[sensores.posF - 1][sensores.posC	  ] = sensores.superficie[1];
			a[sensores.posF - 1][sensores.posC	  ] = sensores.cota[1];

			m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
			a[sensores.posF - 1][sensores.posC - 1] = sensores.cota[2];

			m[sensores.posF	   ][sensores.posC - 1] = sensores.superficie[3];
			a[sensores.posF	   ][sensores.posC - 1] = sensores.cota[3];
			break;
	}
}

list<Action> ComportamientoAuxiliar::AvanzaASaltosDeCaballo()
  {
    list<Action> secuencia;
    secuencia.push_back(WALK);
    secuencia.push_back(WALK);
    secuencia.push_back(TURN_SR);
    secuencia.push_back(TURN_SR);
    secuencia.push_back(WALK);
    return secuencia;
  }

  Action ComportamientoAuxiliar::ComportamientoAuxiliarNivelE(Sensores sensores)
  {
    Action accion = IDLE;
    if (!hayPlan)
    {
      plan = AvanzaASaltosDeCaballo();
      hayPlan = true;
    }
    if (hayPlan and plan.size() > 0)
    {
      accion = plan.front();
      plan.pop_front();
    }
    if (plan.size() == 0) hayPlan = false;

    return accion;
  }

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;

	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
	if (sensores.superficie[0] == 'X') accion = IDLE;
	else if (giro45izq > 0)
	{
		accion = TURN_SR;
		giro45izq--;
	}
	else if (giro45izq < 0)
	{
		accion = TURN_SR;
		giro45izq++;
	}
	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		
		int pos = VeoCasillaInteresanteA(i, c, d);
		switch(pos)
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

	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);

	if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
	if (sensores.superficie[0] == 'X') accion = IDLE;
	else if (giro45izq > 0)
	{
		accion = TURN_SR;
		giro45izq--;
	}
	else if (giro45izq < 0)
	{
		accion = TURN_SR;
		giro45izq++;
	}
	else
	{
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		
		int pos = VeoCasillaInteresanteA_1(i, c, d);
		switch(pos)
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
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}