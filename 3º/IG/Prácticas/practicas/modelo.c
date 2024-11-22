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
modulo modelo.c
    Representación del modelo
    Funciones de dibujo
    Función Idle

*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include "file_ply_stl.h"
#include "lector-jpg.h"
#include "mouse.h"

int modo = GL_FILL;
bool luz = true;

unsigned int idSeleccionado = 0;
bool seleccion = false;
//enum modoDibujo {SELECCION, NORMAL};

void colorSeleccion(unsigned int i)
{
  unsigned char r = i & 0xFF;
  unsigned char g = (i >> 8) & 0xFF;
  unsigned char b = (i >> 16) & 0xFF;

  glColor3ub(r, g, b);
}

void setObjetoSeleccionado(unsigned int id)
{
  idSeleccionado = id;
}

/**	void initModel()
  Inicializa el modelo y de las variables globales
**/
void initModel() {}

void setModo(int m)
{
  modo = m;
  glPolygonMode(GL_FRONT_AND_BACK, modo);
}

void setLuz()
{
  luz = !luz;

  if (modo == GL_FILL and luz) {
    glEnable(GL_LIGHTING);
    printf("Iluminación activada\n");
  } else {
    glDisable(GL_LIGHTING);
    printf("Iluminación desactivada\n");
  }
}

class Cubo : public Objeto3D
{
  private:
    float lado;
    GLuint textid;
    unsigned int idSeleccion;

  public:
    Cubo (float l, unsigned int id)
    {
      lado = l;
      idSeleccion = id;
    }

    unsigned int getId()
    {
      return idSeleccion;
    }

    void draw()
    {
      if(seleccion)
      {
        colorSeleccion(idSeleccion);
      }
      glBegin(GL_QUADS);

      // Cara frontal
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(-lado, -lado,  lado);
      glVertex3f( lado, -lado,  lado);
      glVertex3f( lado,  lado,  lado);
      glVertex3f(-lado,  lado,  lado);

      // Cara trasera
      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(-lado, -lado, -lado);
      glVertex3f(-lado,  lado, -lado);
      glVertex3f( lado,  lado, -lado);
      glVertex3f( lado, -lado, -lado);

      // Cara izquierda
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-lado, -lado, -lado);
      glVertex3f(-lado, -lado,  lado);
      glVertex3f(-lado,  lado,  lado);
      glVertex3f(-lado,  lado, -lado);

      // Cara derecha
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f( lado, -lado, -lado);
      glVertex3f( lado,  lado, -lado);
      glVertex3f( lado,  lado,  lado);
      glVertex3f( lado, -lado,  lado);

      // Cara superior
      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(-lado,  lado, -lado);
      glVertex3f(-lado,  lado,  lado);
      glVertex3f( lado,  lado,  lado);
      glVertex3f( lado,  lado, -lado);

      // Cara inferior
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(-lado, -lado, -lado);
      glVertex3f( lado, -lado, -lado);
      glVertex3f( lado, -lado,  lado);
      glVertex3f(-lado, -lado,  lado);

      glEnd();
    }

    void setTextura(unsigned char * textura, unsigned ancho, unsigned alto)
    {
      glGenTextures(1, &textid);
      glBindTexture(GL_TEXTURE_2D, textid);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);
    
    }

    void drawTextura()
    {
      glEnable(GL_TEXTURE_2D);  // Habilitar texturización

      glBegin(GL_QUADS);

      // Cara frontal (con el número 6 en el dado)
      glTexCoord2f(0.75f, 0.75f); glVertex3f(-0.5f, 0.5f, 0.5f);
      glTexCoord2f(1.0f, 0.75f); glVertex3f(-0.5f, -0.5f, 0.5f);
      glTexCoord2f(1.0f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f);
      glTexCoord2f(0.75f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f);
      
      // Cara trasera (con el número 1 en el dado)
      glTexCoord2f(0.25f, 0.75f); glVertex3f(-0.5f, -0.5f, -0.5f);
      glTexCoord2f(0.5f, 0.75f); glVertex3f(-0.5f, 0.5f, -0.5f);
      glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5f,  0.5f, -0.5f);
      glTexCoord2f(0.25f, 0.5f); glVertex3f(0.5f, -0.5f, -0.5f);

      // Cara superior (con el número 5 en el dado)
      glTexCoord2f(0.0f, 0.75f); glVertex3f(-0.5f, 0.5f, -0.5f);
      glTexCoord2f(0.25f, 0.75f); glVertex3f(-0.5f, 0.5f, 0.5f);
      glTexCoord2f(0.25f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f);
      glTexCoord2f(0.0f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f);

      // Cara inferior (con el número 2 en el dado)
      glTexCoord2f(0.5f, 0.75f); glVertex3f(-0.5f, -0.5f, 0.5f);
      glTexCoord2f(0.75f, 0.75f); glVertex3f(-0.5f, -0.5f, -0.5f);
      glTexCoord2f(0.75f, 0.5f); glVertex3f(0.5f, -0.5f, -0.5f);
      glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f);

      // Cara derecha (con el número 3 en el dado)
      glTexCoord2f(0.5f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f);
      glTexCoord2f(0.75f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f);
      glTexCoord2f(0.75f, 0.25f); glVertex3f(0.5f, -0.5f, 0.5f);
      glTexCoord2f(0.5f, 0.25f); glVertex3f(0.5f, -0.5f, -0.5f);

      // Cara izquierda (con el número 4 en el dado)
      glTexCoord2f(0.5f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
      glTexCoord2f(0.75f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
      glTexCoord2f(0.75f, 0.75f); glVertex3f(-0.5f, -0.5f, -0.5f);
      glTexCoord2f(0.5f, 0.75f); glVertex3f(-0.5f, -0.5f, 0.5f);
      
      glEnd();

      glDisable(GL_TEXTURE_2D);  // Deshabilitar texturización
    }
};

class Piramide : public Objeto3D
{
  private:
    float lado, alto;

  public:
    Piramide(float l, float a)
    {
      lado = l;
      alto = a;
    }

    void draw()
    {
      glBegin(GL_TRIANGLES);

      // Cara frontal
      glNormal3f(0.0, 0.707, 0.707);
      glVertex3f( 0.0,  alto,  0.0); // Punta
      glVertex3f(-lado, -lado,  lado); // Esquina inferior izquierda
      glVertex3f( lado, -lado,  lado); // Esquina inferior derecha

      // Cara derecha
      glNormal3f(0.707, 0.707, 0.0);
      glVertex3f( 0.0,  alto,  0.0); // Punta
      glVertex3f( lado, -lado,  lado); // Esquina inferior derecha
      glVertex3f( lado, -lado, -lado); // Esquina inferior trasera derecha

      // Cara trasera
      glNormal3f(0.0, 0.707, -0.707);
      glVertex3f( 0.0,  alto,  0.0); // Punta
      glVertex3f( lado, -lado, -lado); // Esquina inferior trasera derecha
      glVertex3f(-lado, -lado, -lado); // Esquina inferior trasera izquierda

      // Cara izquierda
      glNormal3f(-0.707, 0.707, 0.0);
      glVertex3f( 0.0,  alto,  0.0); // Punta
      glVertex3f(-lado, -lado, -lado); // Esquina inferior trasera izquierda
      glVertex3f(-lado, -lado,  lado); // Esquina inferior izquierda

      glEnd();

      // Dibujar la base
      glBegin(GL_QUADS);
      glNormal3f(0.0, -1.0, 0.0); // Normal hacia abajo
      glVertex3f(-lado, -lado, -lado); // Esquina inferior izquierda
      glVertex3f( lado, -lado, -lado); // Esquina inferior derecha
      glVertex3f( lado, -lado,  lado); // Esquina inferior trasera derecha
      glVertex3f(-lado, -lado,  lado); // Esquina inferior trasera izquierda

      glEnd();
    }

};

class Malla : Objeto3D
{
  private:
    vector <float> vertices;
    vector <int> caras;
    vector <float> normales_v;
    vector <float> normales_c;
    vector <pair<float, float>> texturas = 
    {
      {0.75f, 0.75f},
      {1.0f, 0.5f},
      {0.75f, 0.5f},
      {1.0f, 0.75f},
      {0.25f, 0.75f},
      {0.5f, 0.5f},
      {0.5f, 0.75f},
      {0.25f, 0.5f},
      {0.0f, 0.5f},
      {0.0f, 0.75f},
      {0.75f, 1.0f},
      {0.5f, 1.0f},
      {0.75f, 0.25f},
      {0.5f, 0.25f}
    };

    GLuint textid;

  public:
    Malla(const char *archivo)
    {
      ply::read(archivo, vertices, caras);
    }

    void calculoNormales()
    {

      float v_0, v_1, v_2;

      float P0_x, P0_y, P0_z;
      float P1_x, P1_y, P1_z;
      float P2_x, P2_y, P2_z;

      float modulo;
      float NX, NY, NZ;

      int size_c = caras.size();

      for(int i = 0; i < size_c; i += 3)
      {
        v_0 = caras[i];
        P0_x = vertices[3 * v_0];                       //cout << "P0_x: " << P0_x << endl;
        P0_y = vertices[3 * v_0 + 1];                   //cout << "P0_y: " << P0_y << endl;
        P0_z = vertices[3 * v_0 + 2];                   //cout << "P0_z: " << P0_z << endl << endl;

        v_1 = caras[i+1];
        P1_x = vertices[3 * v_1];                       //cout << "P1_x: " << P1_x << endl;
        P1_y = vertices[3 * v_1 + 1];                   //cout << "P1_y: " << P1_y << endl;
        P1_z = vertices[3 * v_1 + 2];                   //cout << "P1_z: " << P1_z << endl << endl;

        v_2 = caras[i+2];
        P2_x = vertices[3 * v_2];                       //cout << "P2_x: " << P2_x << endl;
        P2_y = vertices[3 * v_2 + 1];                   //cout << "P2_y: " << P2_y << endl;
        P2_z = vertices[3 * v_2 + 2];                   //cout << "P2_z: " << P2_z << endl << endl;

        float A[3] = {(P1_x - P0_x), (P1_y - P0_y), (P1_z - P0_z)};
        float B[3] = {(P2_x - P0_x), (P2_y - P0_y), (P2_z - P0_z)};

        float PV[3] = {(A[1] * B[2] - A[2] * B[1]), (A[2] * B[0] - A[0] * B[2]), (A[0] * B[1] - A[1] * B[0])};

        modulo = sqrt(PV[0] * PV[0] + PV[1] * PV[1] + PV[2] * PV[2]);
        
        NX = PV[0] / modulo;                            //cout << "Normal X de la cara " << i/3 + 1 << ": " << NX << endl;
        NY = PV[1] / modulo;                            //cout << "Normal Y de la cara " << i/3 + 1 << ": " << NY << endl;
        NZ = PV[2] / modulo;                            //cout << "Normal Z de la cara " << i/3 + 1 << ": " << NZ << endl << endl;

        normales_c.push_back(NX);
        normales_c.push_back(NY);
        normales_c.push_back(NZ);

        normales_v.resize(vertices.size(), 0.0f);

        normales_v[3 * v_0]     += NX;                  //cout << "Normal del vertice " << (v_0) << " sumando la normal NX: " << normales_v[3 * v_0] << endl;
        normales_v[3 * v_0 + 1] += NY;                  //cout << "Normal del vertice " << (v_0) << " sumando la normal NY: " << normales_v[3 * v_0 + 1] << endl;
        normales_v[3 * v_0 + 2] += NZ;                  //cout << "Normal del vertice " << (v_0) << " sumando la normal NZ: " << normales_v[3 * v_0 + 2] << endl << endl;

        normales_v[3 * v_1]     += NX;                  //cout << "Normal del vertice " << (v_1) << " sumando la normal NX: " << normales_v[3 * v_1] << endl;
        normales_v[3 * v_1 + 1] += NY;                  //cout << "Normal del vertice " << (v_1) << " sumando la normal NY: " << normales_v[3 * v_1 + 1] << endl;
        normales_v[3 * v_1 + 2] += NZ;                  //cout << "Normal del vertice " << (v_1) << " sumando la normal NZ: " << normales_v[3 * v_1 + 2] << endl << endl;

        normales_v[3 * v_2]     += NX;                  //cout << "Normal del vertice " << (v_2) << " sumando la normal NX: " << normales_v[3 * v_2] << endl;
        normales_v[3 * v_2 + 1] += NY;                  //cout << "Normal del vertice " << (v_2) << " sumando la normal NY: " << normales_v[3 * v_2 + 1] << endl;
        normales_v[3 * v_2 + 2] += NZ;                  //cout << "Normal del vertice " << (v_2) << " sumando la normal NZ: " << normales_v[3 * v_2 + 2] << endl << endl;        
      }

      int size_v = vertices.size();

      for (int i = 0; i < size_v; i += 3)
      {
        NX = normales_v[i];                             //cout << "NX: " << NX << endl;
        NY = normales_v[i + 1];                         //cout << "NY: " << NY << endl;
        NZ = normales_v[i + 2];                         //cout << "NZ: " << NZ << endl;

        modulo = sqrt(NX * NX + NY * NY + NZ * NZ);
              
        normales_v[i]     /= modulo;                    //cout << "NX despues del modulo: " << normales_v[i] << endl;
        normales_v[i + 1] /= modulo;                    //cout << "NY despues del modulo: " << normales_v[i + 1] << endl;
        normales_v[i + 2] /= modulo;                    //cout << "NZ despues del modulo: " << normales_v[i + 2] << endl << endl;
      }
    }
    
    void draw()
    {
      glBegin(GL_TRIANGLES);

      float v0, v1, v2;

      for(int i = 0; i < caras.size() - 2; i++)
      {
        glNormal3f(normales_c[i], normales_c[i + 1], normales_c[i + 2]);
 
        v0 = caras[i];                                  //cout << v0 << endl;
        v1 = caras[i + 1];                              //cout << v1 << endl;
        v2 = caras[i + 2];                              //cout << v2 << endl;

        glVertex3f(vertices[3 * v0], vertices[3 * v0 + 1], vertices[3 * v0 + 2]);
        glVertex3f(vertices[3 * v1], vertices[3 * v1 + 1], vertices[3 * v1 + 2]);
        glVertex3f(vertices[3 * v2], vertices[3 * v2 + 1], vertices[3 * v2 + 2]);
      }

      glEnd();
    }

    void drawFlat()
    {
      glShadeModel(GL_FLAT);
      glBegin(GL_TRIANGLES);

      float v0, v1, v2;
      float NX, NY, NZ;

      for(int i = 0; i < caras.size(); i += 3)
      { 
        v0 = caras[i];
        v1 = caras[i + 1];
        v2 = caras[i + 2];

        NX = normales_c[i];
        NY = normales_c[i + 1];
        NZ = normales_c[i + 2];

        glNormal3f(NX, NY, NZ);

        glVertex3f(vertices[3 * v0], vertices[3 * v0 + 1], vertices[3 * v0 + 2]);
        glVertex3f(vertices[3 * v1], vertices[3 * v1 + 1], vertices[3 * v1 + 2]);
        glVertex3f(vertices[3 * v2], vertices[3 * v2 + 1], vertices[3 * v2 + 2]); 
      }
      glEnd();
    }

    void drawSmooth()
    {
      glShadeModel(GL_SMOOTH);
      glBegin(GL_TRIANGLES);

      float v0, v1, v2;

      for(int i = 0; i < caras.size(); i++)
      { 
        v0 = caras[i];
        v1 = caras[i + 1];
        v2 = caras[i + 2]; 

        glNormal3f(normales_v[3 * v0], normales_v[3 * v0 + 1], normales_v[3 * v0] + 2);
        glVertex3f(vertices[3 * v0], vertices[3 * v0 + 1], vertices[3 * v0 + 2]);
        
        glNormal3f(normales_v[3 * v1], normales_v[3 * v1 + 1], normales_v[3 * v1] + 2);
        glVertex3f(vertices[3 * v1], vertices[3 * v1 + 1], vertices[3 * v1 + 2]);
        
        glNormal3f(normales_v[3 * v2], normales_v[3 * v2 + 1], normales_v[3 * v2] + 2);
        glVertex3f(vertices[3 * v2], vertices[3 * v2 + 1], vertices[3 * v2 + 2]); 
      }
      glEnd();
    }

    void setTextura(unsigned char * textura, unsigned ancho, unsigned alto)
    {
      glGenTextures(1, &textid);
      glBindTexture(GL_TEXTURE_2D, textid);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);
    }

    void drawTextura()
    {
      glEnable(GL_TEXTURE_2D);  // Habilitar texturización

      glBegin(GL_TRIANGLES);

      // Cara frontal (con el número 6 en el dado)
      glTexCoord2f(texturas[0].first, texturas[0].second); glVertex3f(vertices[18], vertices[19], vertices[20]);
      glTexCoord2f(texturas[1].first, texturas[1].second); glVertex3f(vertices[15], vertices[16], vertices[17]);
      glTexCoord2f(texturas[2].first, texturas[2].second); glVertex3f(vertices[21], vertices[22], vertices[23]);

      glTexCoord2f(texturas[1].first, texturas[1].second); glVertex3f(vertices[15], vertices[16], vertices[17]);
      glTexCoord2f(texturas[0].first, texturas[0].second); glVertex3f(vertices[18], vertices[19], vertices[20]);
      glTexCoord2f(texturas[3].first, texturas[3].second); glVertex3f(vertices[12], vertices[13], vertices[14]);

      // Cara trasera (con el número 1 en el dado)
      glTexCoord2f(texturas[4].first, texturas[4].second); glVertex3f(vertices[6], vertices[7], vertices[8]);
      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[3], vertices[4], vertices[5]);
      glTexCoord2f(texturas[6].first, texturas[6].second); glVertex3f(vertices[0], vertices[1], vertices[2]);

      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[3], vertices[4], vertices[5]);
      glTexCoord2f(texturas[4].first, texturas[4].second); glVertex3f(vertices[6], vertices[7], vertices[8]);
      glTexCoord2f(texturas[7].first, texturas[7].second); glVertex3f(vertices[9], vertices[10], vertices[11]);

      // Cara superior (con el número 5 en el dado)
      glTexCoord2f(texturas[8].first, texturas[8].second); glVertex3f(vertices[9], vertices[10], vertices[11]);
      glTexCoord2f(texturas[4].first, texturas[4].second); glVertex3f(vertices[18], vertices[19], vertices[20]);
      glTexCoord2f(texturas[7].first, texturas[7].second); glVertex3f(vertices[21], vertices[22], vertices[23]);

      glTexCoord2f(texturas[4].first, texturas[4].second); glVertex3f(vertices[18], vertices[19], vertices[20]);
      glTexCoord2f(texturas[8].first, texturas[8].second); glVertex3f(vertices[9], vertices[10], vertices[11]);
      glTexCoord2f(texturas[9].first, texturas[9].second); glVertex3f(vertices[6], vertices[7], vertices[8]);

      // Cara inferior (con el número 2 en el dado)
      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[3], vertices[4], vertices[5]);
      glTexCoord2f(texturas[0].first, texturas[0].second); glVertex3f(vertices[12], vertices[13], vertices[14]);
      glTexCoord2f(texturas[6].first, texturas[6].second); glVertex3f(vertices[0], vertices[1], vertices[2]);

      glTexCoord2f(texturas[0].first, texturas[0].second); glVertex3f(vertices[12], vertices[13], vertices[14]);
      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[3], vertices[4], vertices[5]);
      glTexCoord2f(texturas[2].first, texturas[2].second); glVertex3f(vertices[15], vertices[16], vertices[17]);

      // Cara derecha (con el número 3 en el dado)
      glTexCoord2f(texturas[12].first, texturas[12].second); glVertex3f(vertices[15], vertices[16], vertices[17]);
      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[9], vertices[10], vertices[11]);
      glTexCoord2f(texturas[2].first, texturas[2].second); glVertex3f(vertices[21], vertices[22], vertices[23]);

      glTexCoord2f(texturas[5].first, texturas[5].second); glVertex3f(vertices[9], vertices[10], vertices[11]);
      glTexCoord2f(texturas[12].first, texturas[12].second); glVertex3f(vertices[15], vertices[16], vertices[17]);
      glTexCoord2f(texturas[13].first, texturas[13].second); glVertex3f(vertices[3], vertices[4], vertices[5]);

      // Cara izquierda (con el número 4 en el dado)
      glTexCoord2f(texturas[10].first, texturas[10].second); glVertex3f(vertices[12], vertices[13], vertices[14]);
      glTexCoord2f(texturas[6].first, texturas[6].second); glVertex3f(vertices[6], vertices[7], vertices[8]);
      glTexCoord2f(texturas[11].first, texturas[11].second); glVertex3f(vertices[0], vertices[1], vertices[2]);

      glTexCoord2f(texturas[6].first, texturas[6].second); glVertex3f(vertices[6], vertices[7], vertices[8]);
      glTexCoord2f(texturas[10].first, texturas[10].second); glVertex3f(vertices[12], vertices[13], vertices[14]);
      glTexCoord2f(texturas[0].first, texturas[0].second); glVertex3f(vertices[18], vertices[19], vertices[20]);

      glEnd();

      glDisable(GL_TEXTURE_2D);  // Deshabilitar texturización
    }
};

class Ejes:Objeto3D 
{ 
  public: 
    float longitud = 30;
    // Dibuja el objeto
    void draw( )
    {
      glDisable (GL_LIGHTING);
      glBegin (GL_LINES);
      {
        glColor3f (0, 1, 0);
        glVertex3f (0, 0, 0);
        glVertex3f (0, longitud, 0);

        glColor3f (1, 0, 0);
        glVertex3f (0, 0, 0);
        glVertex3f (longitud, 0, 0);

        glColor3f (0, 0, 1);
        glVertex3f (0, 0, 0);
        glVertex3f (0, 0, longitud);
      }
      glEnd ();
      if (modo == GL_FILL && luz) glEnable (GL_LIGHTING);
    }
}; 

Ejes ejesCoordenadas;

Cubo cubo = Cubo (1.0, 1);
/*Piramide piramide = Piramide(1.0, 5.0);

Malla cubo_ply = Malla("./ply/cubo.ply");
Malla coche = Malla("./ply/big_dodge.ply");
Malla dado = Malla("./ply/cubo.ply");

Cubo C1 = Cubo(0.5);
Cubo C2 = Cubo(0.5);
Cubo C3 = Cubo(0.5);

unsigned ancho = 1024, alto = 1024;
unsigned char * textura = LeerArchivoJPEG("./jpg/dado.jpg", ancho, alto);*/

/*void P1(float color1[4], float color2[4])
{
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);  
  cubo.draw();

  glTranslatef(5,0,0);
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  piramide.draw();
}*/

/*void P2(float color1[4], float color2[4])
{
  coche.calculoNormales();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
  //coche.draw();
  coche.drawFlat();
  //coche.drawSmooth();

  glTranslatef(10,0,0);

  cubo_ply.calculoNormales();
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  cubo_ply.draw();
}*/

/*void P4(float color[4])
{
  cubo.setTextura(textura, ancho, alto);
  cubo.drawTextura();

  glTranslatef(2,0,0);
  dado.calculoNormales();
  dado.setTextura(textura, ancho, alto);
  dado.drawTextura();

  glTranslatef(2,0,0);
  glMaterialfv (GL_FRONT, GL_AMBIENT, color);
  C1.draw();

  glTranslatef(2,0,0);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, color);
  C2.draw();

  glTranslatef(2,0,0);
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  C3.draw();
}*/

void P5()
{
  cubo.draw();
  glutMouseFunc(clickRaton);
}

/**	void Dibuja( void )
  Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/

void DibujaEscena(bool seleccion)
{
  if(seleccion)
  {
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE);
  }
  else
  {
    ejesCoordenadas.draw();
  }

  //P1(magenta, cian);
  //P2(cian, magenta);
  //P4(cian);
  P5();

  glEnable(GL_LIGHTING);
	glEnable(GL_DITHER);
}

void Dibuja (void)
{
  glPushMatrix ();
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer
  transformacionVisualizacion ();

  static GLfloat  luz[4] = { 5.0, 5.0, 10.0, 0.0 };	
  glLightfv (GL_LIGHT0, GL_POSITION, luz);

  float cian[4] = {0.0, 0.75, 1.0, 1};
  //float magenta[4] = {1.0, 0.0, 1.0, 1};

  glMaterialfv (GL_FRONT, GL_AMBIENT, cian);

  DibujaEscena(false);
    
  glPopMatrix ();	// Desapila la transformacion geometrica
  glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}

/**	void idle()
  Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.
**/

void idle (int v)
{
  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
}