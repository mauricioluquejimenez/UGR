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

	practicasIG.h
*/

#include  "modelo.h"
#include  "mouse.h"
#include  "entradaTeclado.h"
#include  "visual.h"
#include  "file_ply_stl.h"

extern float coche_rotacion, baymax_rotacion, cursor_rotacion, sims_rotacion, trofeo_rotacion;
extern float coche_x, coche_z, baymax_x, baymax_z, cursor_y, sims_x, sims_z, trofeo_y;

extern float arm_r_rot, arm_l_rot;
extern float leg_r_rot, leg_l_rot;

extern void set_animacion();