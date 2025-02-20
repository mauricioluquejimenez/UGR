/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2023-24

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

	 mouse.c
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "practicasIG.h"


#define __mouse__ 
#include "mouse.h"

int MOUSE_LEFT_DOWN=0;
int MOUSE_MIDDLE_DOWN=0;
int MOUSE_RIGHT_DOWN=0;
int MOUSE_X, MOUSE_Y;

int ISINTERACTING;

extern bool textura;
extern bool seleccion;

int pick(int x, int y)
{
	GLint viewport[4];
	unsigned char data[4];

	glGetIntegerv (GL_VIEWPORT, viewport);
	glDisable(GL_DITHER);
	glDisable(GL_LIGHTING);

	DibujaEscena(false);

	glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);

	glFlush();
	glFinish();

	glReadPixels(x,viewport[3]-y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,data);

	unsigned int id = data[0] + (data[1] << 8) + (data[2] << 16);
	cout << "Picked color values: R=" << (int)data[0] << " G=" << (int)data[1] << " B=" << (int)data[2] << endl;

	glutPostRedisplay();

	return id;
}

/**

Procedimiento para gestionar los eventos de pulsacion de los botones del raton.

Argumentos:

boton: Codigo glut del boton que ha manipulado el usuario.

estado: Estado en el que esta el boton que se ha pulsado.

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

 void clickRaton( int boton, int estado, int x, int y )
{
	ISINTERACTING=1;
	if(boton==GLUT_LEFT_BUTTON && estado==GLUT_DOWN)
	{
		MOUSE_LEFT_DOWN=1;
		MOUSE_X=x;
		MOUSE_Y=y;

		if(!textura)
		{
			seleccion = true;
			unsigned int id = pick(x, y);

        	setObjetoSeleccionado(id);
			if (id == 0) seleccion = false;

			cout << "x: " << x << " y: " << y << endl;
			cout << "Selected object with ID: " << id << endl;
		}		
	}
	else if(boton==GLUT_MIDDLE_BUTTON && estado==GLUT_DOWN)
	{
		MOUSE_MIDDLE_DOWN=1;
		MOUSE_X=x;
		MOUSE_Y=y;
		seleccion = false;
	}
	else if(boton==GLUT_RIGHT_BUTTON && estado==GLUT_DOWN)
	{
		MOUSE_RIGHT_DOWN=1;
		MOUSE_X=x;
		MOUSE_Y=y;
		seleccion = false;
	}
	else
	{	
		MOUSE_LEFT_DOWN=0;
		MOUSE_MIDDLE_DOWN=0;
		MOUSE_RIGHT_DOWN=0;
		ISINTERACTING=0;
		seleccion = false;
	}
	glutPostRedisplay();
}


/**

Procedimiento para gestionar los eventos de movimiento del raton.

Argumentos:

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/

void RatonMovido( int x, int y )
{
char texto[30];
float ax,ay,az,d;
getCamara (ax, ay, az, d);
	if(MOUSE_LEFT_DOWN) {
		if(x!=MOUSE_X) ay+=x-MOUSE_X;
		if(y!=MOUSE_Y) ax+=y-MOUSE_Y;
		setCamara (ax, ay, az,  d);
		}
	else if(MOUSE_RIGHT_DOWN) {
	//   if(x!=MOUSE_X) z_camara=10.0*(anchoVentana/2-MOUSE_X)/(x+1.0);
	if(y!=MOUSE_Y) {
		d+=100.0*(y-MOUSE_Y)/200; //escalar el zoom
		setCamara (ax,  ay,  az,  d);
		}
	 }
	else if(MOUSE_MIDDLE_DOWN) {
		if(x!=MOUSE_X) az+=x-MOUSE_X;
		if(y!=MOUSE_Y) ax+=y-MOUSE_Y;
		setCamara ( ax,  ay,  az,  d);
		}
	MOUSE_X=x;
	MOUSE_Y=y;
glutPostRedisplay();
}
