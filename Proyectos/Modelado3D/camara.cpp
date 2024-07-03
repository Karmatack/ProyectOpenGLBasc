#include "camara.h"

// Implementación de las variables globales
GLfloat camaraX = 0.0, camaraY = 0.0, camaraZ = 0.0;
GLfloat anguloCamaraX = 0.0, anguloCamaraY = 0.0;
GLfloat ortoIzquierda = -500.0, ortoDerecha = 500.0, ortoAbajo = -500.0, ortoArriba = 500.0;
GLfloat ortoCerca = -15000.0, ortoLejos = 15000.0;

int ultimoMouseX, ultimoMouseY;
bool mouseIzquierdoAbajo = false;
bool mouseDerechoAbajo = false;

void actualizarCamara() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ortoIzquierda, ortoDerecha, ortoAbajo, ortoArriba, ortoCerca, ortoLejos);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-camaraX, -camaraY, -camaraZ);
    glRotatef(anguloCamaraX, 1.0f, 0.0f, 0.0f);
    glRotatef(anguloCamaraY, 0.0f, 1.0f, 0.0f);
}

void manejarMouse(int boton, int estado, int x, int y) {
    if (boton == GLUT_LEFT_BUTTON) {
        if (estado == GLUT_DOWN) {
            mouseIzquierdoAbajo = true;
            ultimoMouseX = x;
            ultimoMouseY = y;
        }
        else if (estado == GLUT_UP) {
            mouseIzquierdoAbajo = false;
        }
    }
    else if (boton == GLUT_RIGHT_BUTTON) {
        if (estado == GLUT_DOWN) {
            mouseDerechoAbajo = true;
            ultimoMouseX = x;
            ultimoMouseY = y;
        }
        else if (estado == GLUT_UP) {
            mouseDerechoAbajo = false;
        }
    }
    else if (boton == 3) { // Scroll up
        ortoIzquierda *= 0.9f;
        ortoDerecha *= 0.9f;
        ortoAbajo *= 0.9f;
        ortoArriba *= 0.9f;
    }
    else if (boton == 4) { // Scroll down
        ortoIzquierda *= 1.1f;
        ortoDerecha *= 1.1f;
        ortoAbajo *= 1.1f;
        ortoArriba *= 1.1f;
    }
    glutPostRedisplay();
}

void manejarMovimientoMouse(int x, int y) {
    if (mouseIzquierdoAbajo) {
        anguloCamaraY += (x - ultimoMouseX);
        anguloCamaraX += (y - ultimoMouseY);
        ultimoMouseX = x;
        ultimoMouseY = y;
    }
    if (mouseDerechoAbajo) {
        camaraX += (x - ultimoMouseX) * 0.8f;
        camaraY -= (y - ultimoMouseY) * 0.8;
        ultimoMouseX = x;
        ultimoMouseY = y;
    }
    glutPostRedisplay();
}

