#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "practicasIG.h"

using namespace std;

#define __mouse__ 
#include "mouse.h"

extern bool seleccion;

int MOUSE_LEFT_DOWN=0;
int MOUSE_MIDDLE_DOWN=0;
int MOUSE_RIGHT_DOWN=0;
int MOUSE_X, MOUSE_Y;

int ISINTERACTING;

int pick(int x, int y)
{
    unsigned char data[4];
    
    DibujaEscena(true);
    
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    unsigned int id = data[0] + (data[1] << 8) + (data[2] << 16);
    
    cout << "Picked color values: R=" << (int)data[0] << " G=" << (int)data[1] << " B=" << (int)data[2] << endl;
    
    glutPostRedisplay();
    return id;
}

void clickRaton(int boton, int estado, int x, int y)
{
    ISINTERACTING=1;
    if(boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        MOUSE_LEFT_DOWN = 1;
        MOUSE_X = x;
        MOUSE_Y = y;

        unsigned int idSeleccionado = pick(x, y);
        cout << "id: " << idSeleccionado << endl;
        
        cout << "x: " << x << " y: " << y << endl;
        if(idSeleccionado > 0)
        {
            setObjetoSeleccionado(idSeleccionado);
            seleccion = true;
            cout << "Selected object with ID: " << idSeleccionado << endl;
        } else seleccion = false;
    }
    else if(boton == GLUT_MIDDLE_BUTTON && estado == GLUT_DOWN) {
        MOUSE_MIDDLE_DOWN = 1;
        MOUSE_X = x;
        MOUSE_Y = y;
    }
    else if(boton == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {
        MOUSE_RIGHT_DOWN=1;
        MOUSE_X = x;
        MOUSE_Y = y;
    }
    else {    
        MOUSE_LEFT_DOWN = 0;
        MOUSE_MIDDLE_DOWN = 0;
        MOUSE_RIGHT_DOWN = 0;
        ISINTERACTING = 0;
        seleccion = false;
    }
    glutPostRedisplay();
}

void RatonMovido(int x, int y)
{
    float ax = 0, ay = 0, az = 0, d = 0;
    getCamara(ax, ay, az, d);
    if(MOUSE_LEFT_DOWN) {
        if(x!=MOUSE_X) ay+=x-MOUSE_X;
        if(y!=MOUSE_Y) ax+=y-MOUSE_Y;
        setCamara(ax, ay, az, d);
    }
    else if(MOUSE_RIGHT_DOWN) {
        if(y!=MOUSE_Y) {
            d+=100.0*(y-MOUSE_Y)/200;
            setCamara(ax, ay, az, d);
        }
    }
    else if(MOUSE_MIDDLE_DOWN) {
        if(x!=MOUSE_X) az+=x-MOUSE_X;
        if(y!=MOUSE_Y) ax+=y-MOUSE_Y;
        setCamara(ax, ay, az, d);
    }
    MOUSE_X=x;
    MOUSE_Y=y;
    glutPostRedisplay();
}
