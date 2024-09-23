/*

	J.C. Torres
	Dpto. Lenguajes y Sistemas Informáticos
	E.T.S.I. Informática
	Univ. de Granada

    Dibuja un cubo girando
    compilar con: gcc cubo.c -lglut -lGLU -lGL -o cubo
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float roty=30.0;

void plano( float t )
//Construye un plano horizontal de tamano t x t
{
  
  glBegin( GL_QUADS ); 
    glNormal3f( 0.0, 1.0, 0.0 );     
    glVertex3f(  t, 0,  t );
    glVertex3f(  t, 0, -t );
    glVertex3f( -t, 0, -t );
    glVertex3f( -t, 0,  t );
  glEnd();
}

void Dibuja( )
{
  float pos[4] = {5.0, 5.0, 10.0, 0.0 };
  float morado[4]={0.8,0,1,1}, verde[4]={0,1,0,1};
  glClearColor(1,1,1,1); // Fondo blanco
  glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  glTranslatef(-0.5,-0.5,-100);
  glLightfv( GL_LIGHT0, GL_POSITION, pos );
  glRotatef( 20, 1.0, 0.0, 0.0 );
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, verde );
  plano(30);
  glRotatef( roty, 0.0, 1.0, 0.0 );
  glTranslatef(0,5,0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, morado );
  glutSolidCube(10);
  glutSwapBuffers();
}
/* ------------------------------------------------------------------------- */

void Ventana(GLsizei ancho,GLsizei alto)
{ // Se ejecuta cuando se cambia la ventana, recibe el ancho y alto de la ventana X
    float D=ancho; if(D<alto) D=alto;
    glViewport(0,0,ancho,alto); //fija el area de dibujo en la ventana 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ancho/D,ancho/D,-alto/D,alto/D,5,250);
    glMatrixMode(GL_MODELVIEW);
}

void idle()
{
  roty +=0.15;
  glutPostRedisplay();
}

int main( int argc, char *argv[] )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutCreateWindow("IG: cubo");
  glutDisplayFunc( Dibuja );
  glutReshapeFunc( Ventana );
  glutIdleFunc(idle);
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return 0;
}












