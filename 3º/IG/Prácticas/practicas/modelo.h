extern bool seleccion;
extern bool luz;
extern int modo;

void colorSeleccion(unsigned int i);

void setObjetoSeleccionado(unsigned int id);

void DibujaEscena(bool seleccion);

/**
	Funcion de redibujado. Se ejecuta con los eventos postRedisplay
**/
void Dibuja(void);

/**
	Funcion de fondo
**/
void idle(int v);

/**
	Funcion de inicializacion del modelo y de las variables globales
**/
void initModel();

/**
  	Funciones de la P1
**/
void setModo(int m);
void setLuz();
void P1(float color1[4], float color2[4]);

/**
  	Funciones de la P2
**/
void P2(float color1[4], float color2[4]);

/**
  	Funciones de la P4
**/
void P4(float color[4]);

/**
  	Funciones de la P5
**/
void P5();

class Objeto3D 
{
	public: 
		virtual void draw() = 0; // Dibuja el objeto
};
