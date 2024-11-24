#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "practicasIG.h"
#include "file_ply_stl.h"
#include "lector-jpg.h"
#include <vector>

using namespace std;

// Global variables
bool seleccion = false;
bool luz = true;
int modo = GL_FILL;
unsigned int idSeleccionado = 0;

class Ejes : public Objeto3D 
{ 
  public: 
    float longitud = 30;
    void draw()
    {
      glDisable(GL_LIGHTING);
      glBegin(GL_LINES);
      {
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, longitud, 0);

        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(longitud, 0, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, longitud);
      }
      glEnd();
      if (modo == GL_FILL && luz) glEnable(GL_LIGHTING);
    }
};

// Declare ejesCoordenadas before use


class Malla : public Objeto3D
{
  private:
    vector<float> vertices;
    vector<int> caras;
    vector<float> normales_v;
    vector<float> normales_c;
    vector<pair<float, float>> texturas = 
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
    unsigned int idSeleccion;

  public:
    Malla(const char *archivo, unsigned int id)
    {
      ply::read(archivo, vertices, caras);
      idSeleccion = id;
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
      if(seleccion)
      {
        glDisable(GL_LIGHTING);
        colorSeleccion(idSeleccion);
        glColor3f(0.0, 0.0, 1.0);
      }
      else
      {
        if(luz && modo == GL_FILL)
        {
          glEnable(GL_LIGHTING);
          float material_diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};  // Green
          float material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
          float material_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
          float shininess = 32.0f;
          
          glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
          glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
          glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
          glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        }
        else
        {
          glDisable(GL_LIGHTING);
        }
      }

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
      // ... (rest of setTextura implementation remains the same)
    }

    void drawTextura()
    {
      // ... (rest of drawTextura implementation remains the same)
    }
};

void colorSeleccion(unsigned int id)
{    unsigned char r = (id) & 0xFF;
    unsigned char g = (id >> 8) & 0xFF;
    unsigned char b = (id >> 16) & 0xFF;

    glColor3ub(r, g, b);
}

void setObjetoSeleccionado(unsigned int id)
{
    idSeleccionado = id;
}

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
        Cubo(float l, unsigned int id)
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
                glDisable(GL_LIGHTING);
                colorSeleccion(idSeleccion);
                glColor3f(0.0, 1.0, 0.0);
            }
            else
            {
                if(luz && modo == GL_FILL)
                {
                    glEnable(GL_LIGHTING);
                    float material_diffuse[] = {1.0f, 0.25f, 0.0f, 1.0f};  // Cyan
                    float material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
                    float material_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
                    float shininess = 32.0f;
                    
                    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
                    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
                }
                else
                {
                    glDisable(GL_LIGHTING);
                }
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
      if(seleccion)
      {
        glDisable(GL_TEXTURE_2D);
        colorSeleccion(idSeleccion);
        glColor3f(0.0, 0.5, 0.5);
      }
      else
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
      
    }
};

static Ejes ejesCoordenadas;
static Cubo cubo(0.5, 1);
static Cubo dado (0.5, 2); 
static Malla coche = Malla("./ply/big_dodge.ply", 3);
unsigned int ancho = 1024, alto = 1024;
unsigned char * textura = LeerArchivoJPEG("./jpg/dado.jpg", ancho, alto);

void P5()
{
  glPushMatrix();
  coche.calculoNormales();
  coche.draw();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(15, 0, 0);
  cubo.draw();
  glPopMatrix();

  /*glPushMatrix();
  glTranslatef(2, 0, 0);
  dado.setTextura(textura, ancho, alto);
  dado.drawTextura();
  glPopMatrix();*/

  glutMouseFunc(clickRaton);
}

void DibujaEscena(bool modo_seleccion)
{
    if(modo_seleccion)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDisable(GL_DITHER);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        if(luz && modo == GL_FILL)
        {
            glEnable(GL_LIGHTING);
            ejesCoordenadas.draw();
        }
    }
  
  P5();
}

void Dibuja(void)
{
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    transformacionVisualizacion();

    static GLfloat luz_pos[4] = {5.0, 5.0, 10.0, 0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, luz_pos);

    DibujaEscena(false);

    glPopMatrix();
    glutSwapBuffers();
}

void idle(int v)
{
    glutPostRedisplay();
    glutTimerFunc(30, idle, 0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
