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
		frontera = false;
		giro180 = false;
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
	int n_casillas; // Número de casillas recorridas
	bool frontera; // verdadero cuando se activa el sensor CNY70
	bool giro180; // verdadero durante un giro de 180 grados
};

string ActionStr(Agent::ActionType);

#endif
