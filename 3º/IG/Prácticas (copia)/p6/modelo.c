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
#include "lector-jpg.h"
#include "practicasIG.h"

int modo = GL_FILL;
bool luz = true;
bool modo_animacion = false;
bool textura = false;
bool seleccion = false;
unsigned int idSeleccionado = 0;

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

void set_animacion()
{
  modo_animacion = !modo_animacion;

  if (modo_animacion)cout << "modo_animacion: true" << endl;
  else cout << "modo_animacion: false" << endl;
}

void set_textura()
{
  textura = !textura;
  if (textura)
  {
    seleccion = false; // Asegúrate de que la selección esté desactivada al cargar la textura
    glEnable(GL_TEXTURE_2D); // Habilitar texturas
  }

  cout << "Estado de textura: " << textura << endl;
}

void colorSeleccion(unsigned int i)
{
  //cout << "Color seleccionado: " << i << endl;

  unsigned char r = (i & 0xFF);
  unsigned char g = (i >> 8 )& 0xFF;
  unsigned char b = (i >> 16) & 0xFF;

  glColor3ub(r,g,b);
}

void setObjetoSeleccionado(unsigned int id)
{
  idSeleccionado = id;
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

    GLuint & getTextId()
    {
      return textid;
    }

    unsigned int & getIdSeleccion()
    {
      return idSeleccion;
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

    void setTextura(unsigned char * textura, unsigned ancho, unsigned alto)
    {
      if (textid != 0) glDeleteTextures(1, &textid);

      glGenTextures(1, &textid);
      glBindTexture(GL_TEXTURE_2D, textid);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);
    }

    void drawTextura()
    {
      glEnable(GL_CULL_FACE);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textid);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glDisable(GL_LIGHTING);

      glBegin(GL_QUADS);

      // Cara superior (Carretera) - Coordenadas normalizadas [0, 1]
      glNormal3f(0.0, 1.0, 0.0);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-lado,  lado, -lado);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-lado,  lado,  lado);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( lado,  lado,  lado);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( lado,  lado, -lado);

      glEnd();

      glDisable(GL_TEXTURE_2D);
    }
};

class Piramide : public Objeto3D
{
  private:
    float lado, alto;

  public:
    Piramide() : lado(1.0f), alto(1.0f) {}
    
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
    vector <pair<float, float>> texturas;

    float forward = true;
    GLuint textid;
    unsigned int idSeleccion;

  public:
    Malla() : textid(0), idSeleccion(0) {}

    Malla(const char *archivo, unsigned int id)
    {
      ply::read(archivo, vertices, caras);
      idSeleccion = id;
    }

    GLuint & getTextId()
    {
      return textid;
    }

    unsigned int & getIdSeleccion()
    {
      return idSeleccion;
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
    if(seleccion and idSeleccionado > 0)
      {
        glDisable(GL_LIGHTING);
        colorSeleccion(idSeleccion);
      }
      else
      {
        if(luz && modo == GL_FILL)
        {
          glEnable(GL_LIGHTING);
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
      if(seleccion and idSeleccionado > 0)
      {
        glDisable(GL_LIGHTING);
        colorSeleccion(idSeleccion);
      }
      else
      {
        if(luz && modo == GL_FILL)
        {
          glEnable(GL_LIGHTING);
        }
      }

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
      if(seleccion and idSeleccionado > 0)
      {
        glDisable(GL_LIGHTING);
        colorSeleccion(idSeleccion);
      }
      else
      {
        if(luz && modo == GL_FILL)
        {
          glEnable(GL_LIGHTING);
        }
      }

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

    void animacion(float &x, float &z, float &rotacion, float giro)
    {
      if (modo_animacion)
      {
        float radianes = rotacion * (M_PI / 180.0f);
        float velocidad = 0.1f;

        if (forward)
        {
          x += velocidad * cos(radianes);
          z -= velocidad * sin(radianes);

          if (x >= 10.0f or x <= -5.0f)
          {
            forward = false;
            rotacion += giro;
          }
          
        } else forward = true;
      }
    }
    
    void setTextura(unsigned char * textura, unsigned ancho, unsigned alto)
    {
      if (textid != 0) glDeleteTextures(1, &textid);

      glGenTextures(1, &textid);
      glBindTexture(GL_TEXTURE_2D, textid);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, textura);

      delete[] textura;
    }

    void drawTextura()
    {
      if (textid != 0 && !texturas.empty())
      {
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, textid);

        glBegin(GL_TRIANGLES);

        for (int i = 0; i < caras.size(); i += 3)
        {
          glNormal3f(normales_c[i], normales_c[i + 1], normales_c[i + 2]);

          for (int j = 0; j < 3; j++)
          {
            int v = caras[i + j];
            glTexCoord2f(texturas[v].first, texturas[v].second);
            glVertex3f(vertices[3 * v], vertices[3 * v + 1], vertices[3 * v + 2]);
          }
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);

        glDeleteTextures(1, &textid);
      }
    }
};
class Baymax : Objeto3D
{
  private:
    Malla body;
    Malla arm_l;
    Malla arm_r;
    Malla leg_l;
    Malla leg_r;
    bool forward = true;
    unsigned int idSeleccion;  

  public:
    Baymax() {}

    Baymax(Malla body, Malla arm_l, Malla arm_r, Malla leg_l, Malla leg_r, unsigned int id)
    {
      this->body = body;
      this->arm_l = arm_l;
      this->arm_r = arm_r;
      this->leg_l = leg_l;
      this->leg_r = leg_r;
      idSeleccion = id;
    }

    unsigned int & getIdSeleccion()
    {
      return idSeleccion;
    }

    void draw()
    {
      glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.1f, 0.1f, 0.1f);

      body.calculoNormales();
      body.drawFlat();

      // Dibujar brazo izquierdo
      glPushMatrix();
        glRotatef(arm_l_rot, 0.5f, 0.0f, 0.5f); // Rotar alrededor del eje X
        arm_l.calculoNormales();
        arm_l.drawFlat();
      glPopMatrix();

      // Dibujar brazo derecho
      glPushMatrix();
        glRotatef(arm_r_rot, 0.5f, 0.0f, 0.5f); // Rotar alrededor del eje X
        arm_r.calculoNormales();
        arm_r.drawFlat();
      glPopMatrix();

      // Dibujar pierna izquierda
      glPushMatrix();
        glRotatef(leg_l_rot, 1.0f, 0.0f, 0.0f); // Rotar alrededor del eje X
        leg_l.calculoNormales();
        leg_l.drawFlat();
      glPopMatrix();

      // Dibujar pierna derecha
      glPushMatrix();
        glRotatef(leg_r_rot, 1.0f, 0.0f, 0.0f); // Rotar alrededor del eje X
        leg_r.calculoNormales();
        leg_r.drawFlat();
      glPopMatrix();

      glPopMatrix();
    }

    void animacion(float &x, float &z, float &rotacion, float giro)
    {
      if (modo_animacion)
      {
        float radianes = rotacion * (M_PI / 180.0f);
        float velocidad = 0.1f;

        static bool arm_l_forward = true;
        static bool arm_r_forward = false;
        static bool leg_l_forward = true;
        static bool leg_r_forward = false;

        if (forward)
        {
          x += velocidad * cos(radianes);
          z -= velocidad * sin(radianes);

          if (arm_l_forward)
          {
            arm_l_rot += 0.5f;
            if (arm_l_rot >= 10.0f) arm_l_forward = false;
          }
          else
          {
            arm_l_rot -= 0.5f;
            if (arm_l_rot <= -10.0f) arm_l_forward = true;
          }

          // Control de rotación del brazo derecho
          if (arm_r_forward)
          {
            arm_r_rot += 0.5f;
            if (arm_r_rot >= 10.0f) arm_r_forward = false;
          }
          else
          {
            arm_r_rot -= 0.5f;
            if (arm_r_rot <= -10.0f) arm_r_forward = true;
          }

          // Control de rotación de la pierna izquierda
          if (leg_l_forward)
          {
            leg_l_rot += 0.5f;
            if (leg_l_rot >= 10.0f) leg_l_forward = false;
          }
          else
          {
            leg_l_rot -= 0.5f;
            if (leg_l_rot <= -10.0f) leg_l_forward = true;
          }

          // Control de rotación de la pierna derecha
          if (leg_r_forward)
          {
            leg_r_rot += 0.5f;
            if (leg_r_rot >= 10.0f) leg_r_forward = false;
          }
          else
          {
            leg_r_rot -= 0.5f;
            if (leg_r_rot <= -10.0f) leg_r_forward = true;
          }
          
          if (x >= 10.0f or x <= -5.0f)
          {
            forward = false;
            rotacion += giro;
          }

        } else forward = true;     
      }
    }
};

class Cursor : Objeto3D
{
  private:
    Piramide superior;
    Piramide inferior;

    bool up = true;

    unsigned int idSeleccion;

  public:
    Cursor() : superior(1.0f, 1.0f), inferior(1.0f, 1.0f) {}

    Cursor(float lado, float alto, unsigned int id)
    {
      superior = Piramide(lado, alto);
      inferior = Piramide(lado, alto);
      idSeleccion = id;
    }

    unsigned int & getIdSeleccion()
    {
      return idSeleccion;
    }

    void draw()
    {
      superior.draw();

      glTranslatef(0.0f, -2.0f, 0.0f);
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      inferior.draw();
    }

    void animacion()
    {
      if (modo_animacion)
      {
        float velocidad = 0.1f; // Velocidad de oscilación
        float limite_superior = 2.0f;
        float limite_inferior = -1.0f;

        if (up)
        {
          cursor_y += velocidad;
          if (cursor_y >= limite_superior) up = false;  // Ahora cambia correctamente
        }
        else
        {
          cursor_y -= velocidad;
          if (cursor_y <= limite_inferior) up = true;  // Ahora cambia correctamente
        }
      }
    }
};

class Sims : Objeto3D
{
  private:
    Baymax baymax;
    Cursor cursor;
    
    unsigned int idSeleccion;

  public:
    Sims(Baymax baymax, Cursor cursor, unsigned int idSeleccion)
    {
      this->baymax = baymax;
      this->cursor = cursor;
      this->idSeleccion = idSeleccion;
    }

    unsigned int & getIdSeleccion()
    {
      return idSeleccion;
    }

    void draw()
    {
      float green[] = {0.0f, 1.0f, 0.0f, 1.0f};
      float selectedGreen[4] = {0.75f, 1.0f, 0.75f, 1};

      glPushMatrix();
        glScalef(0.5f, 0.5f, 0.5f);
        baymax.draw();

        glTranslatef(-0.75f, 3.0f, -0.25f);

        glScalef(0.25f, 0.25f, 0.25f);
        if (idSeleccionado == idSeleccion) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedGreen);
        else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
        glTranslatef(0.0f, cursor_y, 0.0f);
        cursor.draw();
      glPopMatrix();
    }

    void animacion()
    {
      if (modo_animacion)
      {
        float radianes = sims_rotacion * (M_PI / 180.0f);
        float velocidad = 0.1f; // Velocidad de movimiento de Sims

        sims_x += velocidad * cos(radianes);
        sims_z -= velocidad * sin(radianes);

        if (sims_x >= 10.0f || sims_x <= -5.0f)
        {
          sims_rotacion -= 5.0;
        }

        baymax.animacion(baymax_x, baymax_z, baymax_rotacion, -5.0f);
        cursor.animacion();
      }
    }
};

class Trofeo : public Malla
{
  public:
    Trofeo(const char *archivo, unsigned int id)
        : Malla(archivo, id) {}

    void animacion()
    {
      if(modo_animacion)
      {
        trofeo_y += 1.0f;
        if (trofeo_y >= 360.0f) trofeo_y = 0.0f;
      }
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

Cubo road = Cubo (1.0, 0);

Malla coche = Malla("./ply/car.ply", 1);

Malla body = Malla("./ply/baymax_body.ply", 2);
Malla arm_l = Malla("./ply/baymax_arm_l.ply", 2);
Malla arm_r = Malla("./ply/baymax_arm_r.ply", 2);
Malla leg_l = Malla("./ply/baymax_leg_l.ply", 2);
Malla leg_r = Malla("./ply/baymax_leg_r.ply", 2);
Baymax baymax = Baymax(body, arm_l, arm_r, leg_l, leg_r, 2);

Cursor cursor = Cursor(1.0f, 1.0f, 3);
Sims sims = Sims(baymax, cursor, 4);

Malla mesa = Malla("./ply/mesa.ply", 5);
Trofeo trofeo = Trofeo("./ply/trofeo.ply", 6);

unsigned ancho = 1024, alto = 1024;
unsigned char * fondo = LeerArchivoJPEG("./jpg/fondo.jpg", ancho, alto);

unsigned ancho_road = 736, alto_road = 736;
unsigned char * road_texture = LeerArchivoJPEG("./jpg/road.jpeg", ancho_road, alto_road);

void DibujaEscena(bool seleccion)
{
  static GLfloat luz[4] = {5.0f, 5.0f, 10.0f, 0.0}; // Posición de la fuente de luz
  float grey[4] = {0.05f, 0.05f, 0.05f, 1};
  float red[4] = {1.0f, 0.0f, 0.0f, 1};
  float blue[4] = {0.0f, 0.0f, 1.0f, 1};
  float green[4] = {0.0f, 1.0f, 0.0f, 1};
  float yellow[4] = {0.55f, 0.55f, 0.0f, 1};
  float selectedGrey[4] = {0.1f, 0.1f, 0.1f, 1}; // Gris más claro para el coche
  float selectedRed[4] = {1.0f, 0.1f, 0.1f, 1}; // Rojo más claro para el muñeco
  float selectedGreen[4] = {0.75f, 1.0f, 0.75f, 1}; // Verde más claro para la mesa
  float selectedBlue[4] = {0.1f, 0.1f, 1.0f, 1}; // Azul más claro para el objeto seleccionado
  float selectedYellow[4] = {0.75f, 0.75f, 0.5f, 1}; // Amarillo más claro para la silla
  
  glPushMatrix(); // Apila la transformación geométrica actual

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Inicializa el buffer de color y el Z-Buffer
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fija el color de fondo a negro

  transformacionVisualizacion(); // Carga transformación de visualización
  glLightfv(GL_LIGHT0, GL_POSITION, luz); // Declaración de luz. Colocada aquí está fija en la escena

  /*glPushMatrix();
    ejesCoordenadas.draw();
  glPopMatrix();*/

  if(seleccion && idSeleccionado > 0)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    colorSeleccion(idSeleccionado);
  }
  else
  { 
    if(luz && modo == GL_FILL)
    {
      glEnable(GL_DITHER);
      glEnable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
    }
  }

  glPushMatrix();
    glTranslatef(5.0f, -0.55f, 0.0f);
    glScalef(15.0f, 0.15f, 5.0f);

    road.setTextura(road_texture, ancho_road, alto_road);
    road.drawTextura();

  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0f, -0.15f, -1.0f);

    glTranslatef(coche_x, 0.0f, coche_z); // Solo afecta al coche
    glRotatef(coche_rotacion, 0.0f, 1.0f, 0.0f); // Solo afecta al coche
    glScalef(0.1f, 0.1f, 0.1f);

    if (idSeleccionado == coche.getIdSeleccion()) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedGrey);
    else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grey);

    coche.calculoNormales();
    coche.drawFlat();

  glPopMatrix(); // Desapila la transformación del coche

  glTranslatef(0.0f, -0.5f, 1.0f);
  glPushMatrix();
    glScalef(0.5f, 0.5f, 0.5f);
    glTranslatef(baymax_x, 0.0f, baymax_z);
    glRotatef(baymax_rotacion, 0.0f, 1.0f, 0.0f);

    if (idSeleccionado == baymax.getIdSeleccion()) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedRed);
    else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);

    baymax.draw();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(-0.25f, 2.0f, -0.25f);
    glScalef(0.1f, 0.1f, 0.1f);
    //glTranslatef(sims_x, 0.0f, sims_z);
    glRotatef(cursor_rotacion, 0.0f, 1.0f, 0.0f);

    if (idSeleccionado == cursor.getIdSeleccion()) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedGreen);
    else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);

    cursor.draw();
  glPopMatrix();

  glTranslatef(15.0, 0.5, -0.5);
  glRotatef(180, 1.0, 0.0, 0.0);

  glPushMatrix();
    if (idSeleccionado == mesa.getIdSeleccion()) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedGreen);
    else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);    
    mesa.calculoNormales();
    mesa.drawSmooth();
  glPopMatrix();  

  glPushMatrix();
    glTranslatef(0.0, -1.5, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(trofeo_y, 0.0, 0.0, 1.0);
    glScalef(0.5, 0.5, 0.5);
    
    if (idSeleccionado == trofeo.getIdSeleccion()) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, selectedYellow);
    else glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);

    trofeo.calculoNormales();
    trofeo.drawSmooth();
  glPopMatrix();

  glPopMatrix(); // Desapila la transformación geométrica

  glutMouseFunc(clickRaton);
}

/**	void Dibuja( void )
  Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.
**/

void Dibuja(void)
{
  DibujaEscena(false);
  glutSwapBuffers();
}

/**	void idle()
  Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.
**/

void idle (int v)
{
  switch(idSeleccionado)
  {
    case 0:
      coche.animacion(coche_x, coche_z, coche_rotacion, 5.0f);
      sims.animacion();
      trofeo.animacion();
    break;

    case 1:
      coche.animacion(coche_x, coche_z, coche_rotacion, 5.0f);
      trofeo.animacion();
    break;

    case 2:
      baymax.animacion(baymax_x, baymax_z, baymax_rotacion, 5.0f);
      trofeo.animacion(); 
    break;

    case 3:
      cursor.animacion();
      trofeo.animacion();
    break;

    case 5:
    break;
  }

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
}