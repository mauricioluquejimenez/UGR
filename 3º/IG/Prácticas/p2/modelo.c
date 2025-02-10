#include <algorithm>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <float.h>

#include "entradaTeclado.h"
#include "file_ply_stl.h"
#include "practicasIG.h"

int modo = GL_FILL;
bool luz = true;

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

  public:
    Cubo (float l)
    {
      lado = l;
    }

    void draw()
    {
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

  public:
    Malla(){}

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

        float A[3] = {(P1_x - P0_x),
                      (P1_y - P0_y),
                      (P1_z - P0_z)};

        float B[3] = {(P2_x - P0_x),
                      (P2_y - P0_y),
                      (P2_z - P0_z)};

        float PV[3] = {(A[1] * B[2] - A[2] * B[1]), 
                       (A[2] * B[0] - A[0] * B[2]),
                       (A[0] * B[1] - A[1] * B[0])};

        modulo = sqrt(PV[0] * PV[0]
                    + PV[1] * PV[1]
                    + PV[2] * PV[2]);
        
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

      for(int i = 0; i < caras.size(); i += 3)
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

      for(int i = 0; i < caras.size(); i += 3)
      { 
        v0 = caras[i];
        v1 = caras[i + 1];
        v2 = caras[i + 2]; 

        glNormal3f(normales_v[3 * v0], normales_v[3 * v0 + 1], normales_v[3 * v0 + 2]);
        glVertex3f(vertices[3 * v0], vertices[3 * v0 + 1], vertices[3 * v0 + 2]);
        
        glNormal3f(normales_v[3 * v1], normales_v[3 * v1 + 1], normales_v[3 * v1 + 2]);
        glVertex3f(vertices[3 * v1], vertices[3 * v1 + 1], vertices[3 * v1 + 2]);
        
        glNormal3f(normales_v[3 * v2], normales_v[3 * v2 + 1], normales_v[3 * v2 + 2]);
        glVertex3f(vertices[3 * v2], vertices[3 * v2 + 1], vertices[3 * v2 + 2]); 
      }
      glEnd();
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

Cubo cubo = Cubo(100.0);
Cubo road = Cubo (1.0);
Malla coche = Malla("./ply/car.ply");
Malla mesa = Malla("./ply/mesa.ply");
Malla lobo = Malla("./ply/lobo.ply");

/**	void Dibuja( void )
  Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/

void Dibuja()
{
  static GLfloat luz[4] = {5.0f, 5.0f, 10.0f, 0.0}; // Posición de la fuente de luz
  float grey[4] = {0.1f, 0.1f, 0.1f, 1};
  float red[4] = {1.0f, 0.0f, 0.0f, 1};
  float blue[4] = {0.1f, 0.1f, 1.0f, 1};
  float green[4] = {0.0f, 1.0f, 0.0f, 1};
  float yellow[4] = {0.5f, 0.5f, 0.0f, 1};
  
  glPushMatrix(); // Apila la transformación geométrica actual

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Inicializa el buffer de color y el Z-Buffer
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fija el color de fondo a negro

  transformacionVisualizacion(); // Carga transformación de visualización
  glLightfv(GL_LIGHT0, GL_POSITION, luz); // Declaración de luz. Colocada aquí está fija en la escena

  glPushMatrix();
    ejesCoordenadas.draw();
  glPopMatrix();

  // Aislar las transformaciones del coche
  glPushMatrix();
    glScalef(0.5f, 0.5f, 0.5f);
    glTranslatef(-1.0f, 0.0f, 0.0f);
    coche.calculoNormales();
    coche.draw();
  glPopMatrix();

  glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);
    glTranslatef(3.0f, -0.5f, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    mesa.calculoNormales();
    mesa.drawFlat();
  glPopMatrix();  

  glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);
    glTranslatef(-3.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    lobo.calculoNormales();
    lobo.drawSmooth();
  glPopMatrix();

  glPopMatrix(); // Desapila la transformación geométrica

  glutSwapBuffers();
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