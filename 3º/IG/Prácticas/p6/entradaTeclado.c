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

float coche_x = 0.0f, coche_z = 0.0f;
float baymax_x = 0.0f, baymax_z = 0.0f;
float cursor_y = 0.0f;
float sims_x = 0.0f, sims_z = 0.0f;
float trofeo_y = 0.0f;

float coche_rotacion = 0.0f, baymax_rotacion = 0.0f, sims_rotacion = 0.0f, trofeo_rotacion = 0.0f;

float arm_r_rot = 0.0f, arm_l_rot = 0.0f;
float leg_r_rot = 0.0f, leg_l_rot = 0.0f;

void letra (unsigned char k, int x, int y)
{
  float radianes = sims_rotacion * (M_PI / 180.0f);

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

      case 'w': // Mover Baymax hacia adelante
        sims_x += /*velocidad */ cos(radianes);
        sims_z -= /*velocidad */ sin(radianes);
      break;

      case 's': // Mover Baymax hacia atrás
        sims_x -= /*velocidad */ cos(radianes);
        sims_z += /*velocidad */ sin(radianes);
      break;
      
      case 'u':
      case 'U':
        cursor_y += 0.1f;
        if(cursor_y > 1.0f) cursor_y = 1.0f;
      break;

      case 'j':
      case 'J':
        cursor_y -= 0.1f;
        if(cursor_y < -1.0f) cursor_y = -1.0f;
      break;

      case 'a': // Rotar Baymax a la izquierda
        sims_rotacion += 15.0;
        if(sims_rotacion > 360.0) sims_rotacion -= 360.0;
      break;

      case 'd': // Rotar Baymax a la derecha
        sims_rotacion -= 15.0;
        if(sims_rotacion < 0.0) sims_rotacion += 360.0;
      break;

      case 'm': // Brazo derecho hacia adelante
        if (arm_r_rot < 10.0f) arm_r_rot += 1.0f;
      break;
        
      case 'M': // Brazo derecho hacia atrás
        if (arm_r_rot > -10.0f) arm_r_rot -= 1.0f;
      break;

        // Movimiento de la pierna derecha
      case 'n': // Pierna derecha hacia adelante
        if (leg_r_rot > -45.0f) leg_r_rot -= 5.0f;
      break;

      case 'N': // Pierna derecha hacia atrás
        if (leg_r_rot < 0.0f) leg_r_rot += 5.0f;
      break;

        // Movimiento de la pierna izquierda
      case 'b': // Pierna izquierda hacia adelante
        if (leg_l_rot < 45.0f) leg_l_rot += 5.0f;
      break;

      case 'B': // Pierna izquierda hacia atrás
        if (leg_l_rot > 0.0f) leg_l_rot -= 5.0f;
      break;

        // Movimiento del brazo izquierdo
      case 'v': // Brazo izquierdo hacia adelante
        if (arm_l_rot < 10.0f) arm_l_rot += 1.0f;
      break;

      case 'V': // Brazo izquierdo hacia atrás
            if (arm_l_rot > -10.0f) arm_l_rot -= 1.0f;
      break;

      case 'z':
      case 'Z':
        trofeo_rotacion += 15.0;
        if(trofeo_rotacion > 360.0) trofeo_rotacion -= 360.0;
      break;

      case 'A':
        set_animacion();
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
  float radianes = coche_rotacion * (M_PI / 180.0f);
  //float velocidad = 25.0;

  switch (k)
    {
      case GLUT_KEY_UP:
        coche_x += /*velocidad */ cos(radianes);
        coche_z -= /*velocidad */ sin(radianes);
      break;

      case GLUT_KEY_DOWN:
        coche_x -= /*velocidad */ cos(radianes);
        coche_z += /*velocidad */ sin(radianes);
      break;

      case GLUT_KEY_LEFT:
        coche_rotacion += 15.0;
        if(coche_rotacion > 360.0) coche_rotacion -= 360.0;
      break;

      case GLUT_KEY_RIGHT:
        coche_rotacion -= 15.0;
        if(coche_rotacion < 0.0) coche_rotacion += 360.0;
      break;

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