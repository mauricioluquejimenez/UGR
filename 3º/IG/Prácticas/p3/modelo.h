/*	Prácticas de Informática Gráfica

	Grupos C y D				Curso 2023-24
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: 

=======================================================
	G. Arroyo, J.C. Torres 
	Dpto. Lenguajes y Sistemas Informticos
	(Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details 
 http://www.gnu.org/copyleft/gpl.html

=======================================================

	 modelo.h
*/

/**
	Funcion de redibujado. Se ejecuta con los eventos postRedisplay
**/

void Dibuja(void);

/**
	Funcion de fondo
**/
void idle (int v);

/**
	Funcion de inicializacion del modelo y de las variables globales
**/
void initModel ();

/**
  	Funciones de la P1
**/

void setModo(int m);
void setLuz();

/**
  	Funciones de la P2
**/

//void P2(float color1[4], float color2[4]);

/**
  	Funciones de la P3	
**/

void set_animacion();

/**
  	Funciones de la P4
**/

//void P4();

class Objeto3D 
{ 
	public: 
		virtual void draw( ) = 0; // Dibuja el objeto
} ; 
