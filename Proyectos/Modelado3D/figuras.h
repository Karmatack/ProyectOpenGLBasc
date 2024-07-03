#ifndef FIGURAS_H
#define FIGURAS_H

#include <GL/glut.h>

// Declaración de funciones para dibujar figuras en figuras.cpp
void configurarLucesYMateriales(GLfloat light_position[4], GLfloat light_diffuse[4], GLfloat mat_diffuse[4]);
void configurarColorMaterial(GLfloat mat_diffuse[4], const string& color);

void luna(int tamano);
void flotador(int tam,string color);
void ulaula(int tam,string color);
void cilindro(float tam, const string& color);
void esfera(float tam, const string& color);
void tetera(float tam, const string& color);
void dodecaedro(float tam, const string& color);
void octaedro(float tam, const string& color);
// Puedes agregar más declaraciones de funciones aquí según lo que tengas en figuras.cpp

#endif
