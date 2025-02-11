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

=======================================================/
modulo entradaTeclado.c
	Gestion de eventos de teclado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"

void printHelp ()
{

  printf ("\n\n     Prácticas de Informática Gráfica	Curso 2023-2024");
  printf ("\n\n Dpto. Lenguajes y Sistemas Informaticos");
  printf ("\n E.T.S.I. Informatica		Univ. de Granada ");
  printf ("\n");
  printf ("\n Opciones: \n\n");
  printf ("h, H: Imprime informacion de ayuda \n");
  printf ("PgUp, PgDn: avanza y retrocede la cámara \n\n");
  printf ("+,-: avanza y retrocede la cámara \n\n");
  printf ("Teclas de movimiento de cursor: giran la camara\n");
  // Anyade la informacion de las opciones que introduzcas aqui !!       

  printf ("\n Escape: Salir");
  printf ("\n\n\n");
}

/* @teclado ---------------------------------------------------------------- */

/** 		void letra (unsigned char k, int x, int y)

Este procedimiento es llamado por el sistema cuando se pulsa una tecla normal
El codigo k es el ascii de la letra

Para anyadir nuevas ordenes de teclado coloca el correspondiente case.

Parametros de entrada:

k: codigo del caracter pulsado

x:

y:

**/

float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f; // Dirección inicial hacia el origen
float camPosX = 0.0f, camPosY = 0.0f, camPosZ = 10.0f;
float rotxCamara = 30, rotyCamara = 45;
float dCamara = 15;

void letra (unsigned char k, int x, int y)
{
  switch (k)
    {
      case '+':			// acerca la cámara
        dCamara -= 5.0;
      break;

      case '-':			// aleja la cámara
        dCamara += 5.0;
      break;

      case 'h':
      case 'H':
        printHelp ();		// H y h imprimen ayuda
      break;      

      case 'f':
      case 'F':
        setModo(GL_FILL);
      break;

      case 'l':
      case 'L':
        setModo(GL_LINE);
      break;

      case 'p':
      case 'P':
        glPointSize(10);
        setModo(GL_POINT);
      break;

      case 'i':
      case 'I':
        setLuz();
      break;

      case 27:			// Escape  Terminar
        exit (0);

      default:
        return;
    }
  setCamara (rotxCamara, rotyCamara, 0.0, dCamara);
  glutPostRedisplay ();		// Algunas de las opciones cambian paramentros
}				                  // de la camara. Es necesario actualziar la imagen

void especial (int k, int x, int y)
{
  switch (k)
  {
      case GLUT_KEY_PAGE_DOWN:	// acerca la cámara
        dCamara -= 5.0;
      break;

      case GLUT_KEY_PAGE_UP:	// aleja la cámara
        dCamara += 5.0;
      break;

      default:
        return;
    }
  setCamara (rotxCamara, rotyCamara, 0.0, dCamara);
  glutPostRedisplay ();		// Actualiza la imagen (ver proc. letra)
}
