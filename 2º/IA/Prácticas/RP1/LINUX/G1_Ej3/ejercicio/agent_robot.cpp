#include "agent_robot.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	ActionType accion = actFORWARD;
	
	/* ESCRIBA AQUI SUS REGLAS */
	if (giro180)
	{
		cout << "Regla 3: Terminando el giro de 180 grados" << endl;
		accion = actTURN_L;
		giro180 = false;
	}
	else if(CNY70_ and !frontera)
	{
		cout << "Regla 1: Primera vez en la frontera" << endl;
		accion = actTURN_L;
		giro180 = true;
		frontera = true;
		n_casillas = 1;
	}
	else if (CNY70_ and frontera)
	{
		cout << "Regla 2: Terminé de contar" << endl;
		cout << "El tablero es de " << n_casillas << " x " << n_casillas << endl;
		accion = actIDLE;
	}	
	else
	{
		cout << "Regla 4: Regla por defecto" << endl;
		accion = actFORWARD;
		n_casillas++;
	}

	return accion;

}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	CNY70_ = env.isFrontier();
	BUMPER_ = env.isBump();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actBACKWARD: return "BACKWARD";
	case Agent::actPUSH: return "PUSH";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
