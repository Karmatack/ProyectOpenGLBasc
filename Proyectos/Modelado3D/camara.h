#ifndef CAMARA_H
#define CAMARA_H

#include <GL/glut.h>

// Variables globales para la cámara
extern GLfloat camaraX, camaraY, camaraZ;
extern GLfloat anguloCamaraX, anguloCamaraY;
extern GLfloat ortoIzquierda, ortoDerecha, ortoAbajo, ortoArriba;
extern GLfloat ortoCerca, ortoLejos;

// Variables de control del mouse
extern int ultimoMouseX, ultimoMouseY;
extern bool mouseIzquierdoAbajo, mouseDerechoAbajo;

// Funciones de la cámara
void actualizarCamara();
void manejarMouse(int boton, int estado, int x, int y);
void manejarMovimientoMouse(int x, int y);

#endif

