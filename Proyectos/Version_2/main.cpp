#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include "carga_Archivo.h"
#include "camara.h" // Incluir el archivo de la cámara
#include <direct.h>

typedef struct {
    GLubyte *dibujo;
    GLuint bpp;
    GLuint largo;
    GLuint ancho;
    GLuint ID;
} textura;

textura MiTextura;
textura tfondo;
textura tpista;
textura tcampo;

using namespace std;
// Vector de modelos
vector<Modelo> modelos;

string obtenerDirectorioDelEjecutable() {
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL) {
        GetModuleFileName(hModule, path, sizeof(path));
        std::string fullPath(path);
        size_t pos = fullPath.find_last_of("\\/");
        return (string::npos == pos) ? "" : fullPath.substr(0, pos);
    }
    return "";
}
void cambiarAlDirectorioDelEjecutable() {
    string dir = obtenerDirectorioDelEjecutable();
    if (!dir.empty()) {
        _chdir(dir.c_str());
    } else {
        std::cerr << "Error: No se pudo obtener el directorio del ejecutable." << endl;
        exit(1);
    }
}
// Variables para la posición del carro
GLfloat carroX = 0.0f, carroY = 0.0f, carroZ = 0.0f;
GLfloat pistaZ = 0.0f; // Variable para la posición de la pista
GLfloat carroPosInicialZ = 1000.0f; // Posición inicial del carro en el eje Z
bool pistaCineticaActiva = false;

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
            return 0;
        }
        else {
            fclose(archivo);
            return 0;
        }
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
        }
        else {
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

    glGenTextures(1, &imagen[0].ID);
    glBindTexture(GL_TEXTURE_2D, imagen[0].ID);
    if (imagen->bpp == 24) tipo = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, tipo, imagen->ancho, imagen->largo, 0, tipo, GL_UNSIGNED_BYTE, imagen->dibujo);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return 1;
}

void init() {
	cambiarAlDirectorioDelEjecutable();
    // Sistema de iluminación	
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glShadeModel(GL_SMOOTH); // Permite mejorar el dibujo de sombra de los objetos
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0); // Borrando cualquier tipo de profundidad
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D); // Para cubrir los gráficos 3d

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Cargar los archivos .obj con la escala deseada
    modelos.push_back(Modelo("obj\\carro.obj", 1.0f));
    
    // Cargar la textura del fondo
    if (!cargaTGA("fondo2.3.tga", &tfondo)) {
        printf("Error cargando textura del fondo\n");
        exit(0);
    }
    if (!cargaTGA("pista.tga", &tpista)) {
        printf("Error cargando textura de la pista\n");
        exit(0);
    }
    if (!cargaTGA("cesped.tga", &tcampo)) {
        printf("Error cargando textura de la pista\n");
        exit(0);
    }
}

void fondo() {
	glDisable(GL_LIGHTING); // Desactivar la iluminación
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING); // Desactivar la iluminación
    glBindTexture(GL_TEXTURE_2D, tfondo.ID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 1.0);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Reactivar la iluminación
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING); // Reactivar la iluminación
}

void campo(int posx, int posy, int posz){
	int largo = 4000; // Largo del plano
    int ancho = 2000; // Ancho del plano

    glDisable(GL_LIGHTING); // Desactivar la iluminación
    glEnable(GL_TEXTURE_2D);
    glPushMatrix(); // Guardar la matriz actual
    glTranslatef(posx, posy, posz);
    
    glBindTexture(GL_TEXTURE_2D, tcampo.ID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3d(-ancho, 0, 0);
    glTexCoord2f(1.0, 0.0); glVertex3d(ancho, 0, 0);
    glTexCoord2f(1.0, 1.0); glVertex3d(ancho, 0, -largo);
    glTexCoord2f(0.0, 1.0); glVertex3d(-ancho, 0, -largo);
    glEnd();

    glPopMatrix(); // Restaurar la matriz anterior
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING); // Reactivar la iluminación
}

void pista(int posx, int posy, int posz) {
    int largo = 2000; // Largo del plano
    int ancho = 400; // Ancho del plano

    glDisable(GL_LIGHTING); // Desactivar la iluminación
    glEnable(GL_TEXTURE_2D);
    glPushMatrix(); // Guardar la matriz actual
    glTranslatef(posx, posy, posz);
    
    glBindTexture(GL_TEXTURE_2D, tpista.ID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3d(-ancho, 0, 0);
    glTexCoord2f(1.0, 0.0); glVertex3d(ancho, 0, 0);
    glTexCoord2f(1.0, 1.0); glVertex3d(ancho, 0, -largo);
    glTexCoord2f(0.0, 1.0); glVertex3d(-ancho, 0, -largo);
    glEnd();

    glPopMatrix(); // Restaurar la matriz anterior
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING); // Reactivar la iluminación
}

void pistaInfinita(int posy, int posz) {
    glDisable(GL_LIGHTING); // Desactivar la iluminación para la textura
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tpista.ID);
    int largo = 2000; // Largo del plano
    int ancho = 400; // Ancho del plano
    glPushMatrix();
    glTranslatef(0, posy, posz); // Mueve la pista hacia atrás a medida que el carro avanza
    glTranslatef(0, 0, pistaZ); // Mueve la pista hacia atrás a medida que el carro avanza
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3d(-ancho, 0, 0);
    glTexCoord2f(1.0, 0.0); glVertex3d(ancho, 0, 0);
    glTexCoord2f(1.0, 1.0); glVertex3d(ancho, 0, -largo);
    glTexCoord2f(0.0, 1.0); glVertex3d(-ancho, 0, -largo);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING); // Reactivar la iluminación

    // Actualizar la posición de la pista para crear el efecto de movimiento
    pistaZ += 10.0;
    if (pistaZ > 2000) {
        pistaZ = 0; // Resetea la posición de la pista cuando se sale del campo de visión
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, MiTextura.ID);
    fondo();
    campo(0,-8,0);
    campo(0,-8,4000);

    if (pistaCineticaActiva) {
        // pista cinetica
        pistaInfinita(0, -1990);
        pistaInfinita(0, 0);
        pistaInfinita(0, 1990);
    } else {
        // pista estatica
        pista(0, 0, -2000);
        pista(0, 0, 0);
        pista(0, 0, 2000);
        pista(0, 0, 4000);
    }

    actualizarCamara(); // Llamar a la función de la cámara desde camara.h

    for (int i = 0; i < modelos.size(); ++i) {
        glPushMatrix();

        if (i == 0) { // Si es el carro, aplicar las transformaciones
            glTranslatef(carroX, carroY, carroZ + carroPosInicialZ); // Ajustar la posición inicial
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotar el carro 180 grados en el eje Y
        }

        glScalef(modelos[i].escala, modelos[i].escala, modelos[i].escala);

        glBegin(GL_TRIANGLES);
        for (const Cara& cara : modelos[i].caras) {
            // Generar colores aleatorios para cada triángulo
            glColor3f(static_cast<float>(rand()) / RAND_MAX,
                      static_cast<float>(rand()) / RAND_MAX,
                      static_cast<float>(rand()) / RAND_MAX);

            for (int j = 0; j < 3; j++) {
                if (!modelos[i].coordenadasTextura.empty()) {
                    CoordenadaTextura coordTextura = modelos[i].coordenadasTextura[cara.t[j] - 1];
                    glTexCoord2f(coordTextura.u, coordTextura.v);
                }
                if (!modelos[i].normals.empty()) {
                    Normal normal = modelos[i].normals[cara.n[j] - 1];
                    glNormal3f(normal.nx, normal.ny, normal.nz);
                }
                Vertice vertice = modelos[i].vertices[cara.v[j] - 1];
                glVertex3f(vertice.x, vertice.y, vertice.z);
            }
        }
        glEnd();

        // Restablecer el color a blanco después de dibujar los triángulos
        glColor3f(1.0f, 1.0f, 1.0f);

        glPopMatrix();
    }

    glutSwapBuffers();
}

void reshape(int largo, int ancho) {
    glViewport(0, 0, (GLsizei)largo, (GLsizei)ancho);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ortoIzquierda, ortoDerecha, ortoAbajo, ortoArriba, ortoCerca, ortoLejos);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void manejarTeclado(unsigned char key, int x, int y) {
    const GLfloat desplazamiento = 20.0f;
    switch (key) {
        case 'a':
            carroX -= desplazamiento;
            break;
        case 'd':
            carroX += desplazamiento;
            break;
        case 'w':
            if (!pistaCineticaActiva) {
                carroZ -= desplazamiento;
            }
            break;
        case 's':
            carroZ += desplazamiento;
            if (pistaCineticaActiva && (carroZ + carroPosInicialZ) >= 0) { 
				// Punto de retroceso
                pistaCineticaActiva = false;
            }
            break;
        default:
            break;
    }

	// Verificar si el carro está dentro del rango de la pared invisible
    if ((carroZ + carroPosInicialZ) <= 0 &&  !pistaCineticaActiva) { 
		// Punto de cambio de pista
        pistaCineticaActiva = true;
    }

    glutPostRedisplay(); // Redibujar la escena
}

void mouse(int boton, int estado, int x, int y) {
    manejarMouse(boton, estado, x, y); // Llamar a la función de mouse desde camara.h
}

void mouseMotion(int x, int y) {
    manejarMovimientoMouse(x, y); // Llamar a la función de movimiento de mouse desde camara.h
}

void idle() {
    glutPostRedisplay();
}

void inicializarAudio() {
    // Reproducir música en bucle
    PlaySound(TEXT("audio.wav"), NULL, SND_LOOP | SND_ASYNC);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(900, 650);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Proyecto Final/ Juego 1");

    inicializarAudio(); // Inicializar la reproducción de audio

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(manejarTeclado);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}

