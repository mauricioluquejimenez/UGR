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
	if (BUMPER_)
	{
		cout << "Regla 1: Encontré el obstáculo" << endl;
		accion = actTURN_L;
		giro180 = false;
	}
	else if (giro180 == 2 and frontera & 2 == 0)
	{
		cout << "Regla 5: Terminando giro por la derecha" << endl;
		accion = actTURN_R;
		frontera++;
		giro180 = 1;
	}
	else if (giro180 == 2 and frontera & 2 != 0)
	{
		cout << "Regla 6: Terminando giro por la izquierda" << endl;
		accion = actTURN_L;
		frontera++;
		giro180 = 1;
	}
	else if (giro180 == 1 and CNY70_ and frontera % 2 == 0)
	{
		cout << "Regla 8: Evitando esquina en derecha" << endl;
		accion = actTURN_R;
		giro180 = 0;
	}
	else if (giro180 == 1 and CNY70_ and frontera % 2 != 0)
	{
		cout << "Regla 9: Evitando esquina en izquierda" << endl;
		accion = actTURN_L;
		giro180 = 0;
	}
	else if (giro180 == 1)
	{
		cout << "Regla 4: Avance entre giros" << endl;
		accion = actFORWARD;
		giro180 = 2;
	}
	else if(CNY70_ and frontera % 2 == 0)
	{
		cout << "Regla 2: Girando 180º a la derecha" << endl;
		giro180 = 1;
		accion = actTURN_R;
	}
	else if (CNY70_ and frontera % 2 != 0)
	{
		cout << "Regla 2: Girando 180º a la izquierda" << endl;
		giro180 = 1;
		accion = actTURN_L;
	}	
	else
	{
		cout << "Regla 7: Regla por defecto" << endl;
		accion = actFORWARD;
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
