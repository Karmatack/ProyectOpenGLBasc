#ifndef DISENO_H

#define DISENO_H

#include <GL/glut.h>
#include <iostream>

using namespace std;

typedef struct {
    GLubyte *dibujo;
    GLuint bpp;
    GLuint largo;
    GLuint ancho;
    GLuint ID;
} textura;

extern textura cuadrado;
extern textura cuboCasa;
extern textura tpelota_multicolor;
extern textura cesped;
extern textura arbolTronco;
extern textura morada;

void inicializarTextura();
void cuadradoEspacio(int tamano);
void cubo_Casa(int tamano);
void pelota(int tam);
void cuadradocesped(int tamano);
void tronco(float ancho, float alto);
void pelotaMorada(int tam);

#endif // DISENO_H
