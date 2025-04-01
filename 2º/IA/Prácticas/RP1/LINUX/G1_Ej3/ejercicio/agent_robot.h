#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		CNY70_=false;
		BUMPER_=false;

		// Inicialización de variables de estado
		frontera = 0;
		giro180 = 0;
	}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
		actBACKWARD,
		actPUSH,
		actIDLE
	};

	void Perceive(const Environment &env);
	ActionType Think();

private:
	bool CNY70_;
	bool BUMPER_;

	// Variables de estado
	int frontera; // Número de veces en la frontera
	int giro180; // 0: no giro, 1: giro 90 grados, 2: giro 180 grados
};

string ActionStr(Agent::ActionType);

#endif
