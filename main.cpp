#include <GL/glut.h>
#include <iostream>
#include <cstring>

#include <windows.h>
#include <mmsystem.h>

using namespace std;

struct textura {
    GLuint ID;
    int ancho;
    int largo;
    int bpp;
    GLubyte* dibujo;
};

textura fondo;

const char* etiqueta = "Elige un archivo para abrir";


// Declaración de funciones
void inicializarAudio();
void mostrar();
void raton(int, int, int, int);
void inicializar();
void inicializarTextura();
int cargaTGA(char const* nombre, textura* imagen);
void dibujarFondo();
void dibujarBoton(float x, float y, float ancho, float alto, const char* etiqueta);
void abrirArchivo(const char* ruta);

//Inicia 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); 
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(600, 250);
    glutCreateWindow("Proyecto Computación Gráfica");
	
    inicializar();
    inicializarTextura();
	inicializarAudio();
    // Configuración de funciones de callback
    glutDisplayFunc(mostrar);
    glutMouseFunc(raton);

    glutMainLoop();
    return 0;
}

void inicializar() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Color de fondo blanco
    glEnable(GL_TEXTURE_2D); // Habilitar texturas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void inicializarTextura() {
    if (!cargaTGA("tga\\Portada.tga", &fondo)) {
        printf("Error cargando textura Portada \n");
        exit(0);
    }
}

void inicializarAudio() {
    // Reproducir música en bucle
    PlaySound(TEXT("wav\\Sonic-Boom.wav"), NULL, SND_LOOP | SND_ASYNC);
}

int cargaTGA(char const* nombre, textura* imagen) {
    GLubyte cabeceraTGA[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    GLubyte compararTGA[12];
    GLubyte cabecera[6];

    GLuint bytesporpuntos;
    GLuint tamanoimagen;
    GLuint temp, i;
    GLuint tipo = GL_RGBA;

    FILE* archivo = fopen(nombre, "rb");

    if (archivo == NULL ||
        fread(compararTGA, 1, sizeof(compararTGA), archivo) != sizeof(compararTGA) ||
        memcmp(cabeceraTGA, compararTGA, sizeof(compararTGA)) != 0 ||
        fread(cabecera, 1, sizeof(cabecera), archivo) != sizeof(cabecera)) {
        if (archivo == NULL) {
            printf("No se encontró el archivo %s\n", nombre);
        } else {
            fclose(archivo);
        }
        return 0;
    }

    imagen->largo = 256 * cabecera[1] + cabecera[0];
    imagen->ancho = 256 * cabecera[3] + cabecera[2];
    if (imagen->largo <= 0 || imagen->ancho <= 0 || (cabecera[4] != 24 && cabecera[4] != 32)) {
        printf("Datos inválidos\n");
        fclose(archivo);
        return 0;
    }
    imagen->bpp = cabecera[4];
    bytesporpuntos = cabecera[4] / 8;
    tamanoimagen = imagen->largo * imagen->ancho * bytesporpuntos;

    imagen->dibujo = (GLubyte*)malloc(tamanoimagen);
    if (imagen->dibujo == NULL || fread(imagen->dibujo, 1, tamanoimagen, archivo) != tamanoimagen) {
        if (imagen->dibujo != NULL) {
            printf("Error leyendo imagen\n");
            free(imagen->dibujo);
        } else {
            printf("Error asignando memoria\n");
        }
        fclose(archivo);
        return 0;
    }

    for (i = 0; i < (int)tamanoimagen; i += bytesporpuntos) {
        temp = imagen->dibujo[i];
        imagen->dibujo[i] = imagen->dibujo[i + 2];
        imagen->dibujo[i + 2] = temp;
    }
    fclose(archivo);

    glGenTextures(1, &imagen->ID);
    glBindTexture(GL_TEXTURE_2D, imagen->ID);
    if (imagen->bpp == 24) tipo = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, tipo, imagen->ancho, imagen->largo, 0, tipo, GL_UNSIGNED_BYTE, imagen->dibujo);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return 1;
}

void mostrar() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Dibujar fondo
    dibujarFondo();

    // Dibujar etiqueta en la parte superior
    glColor3f(1, 1, 1); // Color para el texto
    glRasterPos2f(-0.4f, 0.9f);
    for (const char* c = etiqueta; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Dibujar botones
    dibujarBoton(-0.3f, 0.3f, 0.6f, 0.2f, "Modelado 3D");
    dibujarBoton(-0.3f, 0.0f, 0.6f, 0.2f, "Conduciendo");
    dibujarBoton(-0.3f, -0.3f, 0.6f, 0.2f, "Space Invaders");

    glutSwapBuffers();
}

void dibujarFondo() {
    glBindTexture(GL_TEXTURE_2D, fondo.ID);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f); 
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);  
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);   
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);  
    glEnd();
}

void raton(int boton, int estado, int x, int y) {
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
        float mx = (float)x / 250.0f - 1.0f;
        float my = 1.0f - (float)y / 250.0f;

        if (mx > -0.3f && mx < 0.3f) {
            if (my > 0.2f && my < 0.4f) {
                abrirArchivo("Proyectos\\Modelado3D\\Modelado3D.exe");
            } else if (my > -0.1f && my < 0.1f) {
                abrirArchivo("Proyectos\\Version_2\\Proyecto_Final_modulo_1.exe");
            } else if (my > -0.4f && my < -0.2f) {
                abrirArchivo("Proyectos\\Space_Invaders\\galaxy.exe");
            }
        }
    }
}

void dibujarBoton(float x, float y, float ancho, float alto, const char* etiqueta) {
    glColor3f(0.0f, 0.0f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + ancho, y);
    glVertex2f(x + ancho, y + alto);
    glVertex2f(x, y + alto);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(x + ancho / 4 - 0.08, y + alto / 2 - 0.05f);
    for (const char* c = etiqueta; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void abrirArchivo(const char* ruta) {
    string comando = "start " + string(ruta);
    system(comando.c_str());
    cout << "Abriendo archivo: " << ruta << endl;
}

//Karmatack

