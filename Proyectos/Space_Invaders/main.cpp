#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <direct.h> // Para _chdir

// Estructura para la textura

using namespace std;

int puntos = 0;

typedef struct {
    GLubyte* imageData;
    GLint bpp;
    GLuint largo;
    GLuint ancho;
    GLuint id;
} Textura;

#define MAX_DISPAROS 100
#define MAX_ENEMIGOS 10


typedef struct {
    GLfloat posX;
    GLfloat posY;
    GLfloat velocidadY;
    bool activo;
} DisparoEnemigo;

#define MAX_ENEMY_SHOTS 50
DisparoEnemigo disparosEnemigos[MAX_ENEMY_SHOTS];

typedef struct {
    GLfloat posX;
    GLfloat posY;
    bool activo;
} Disparo;

typedef struct {
    GLfloat posX;
    GLfloat posY;
    GLfloat escala;
    bool activo;
    GLfloat velocidadX; // Velocidad horizontal
    GLfloat direccionX; // Dirección horizontal (1 para derecha, -1 para izquierda)
} Enemigo;

enum EstadoJuego { MENU, JUGANDO, INSTRUCCIONES, GANADO, PERDIDO };
enum EstadoJuego estadoActual = MENU;


Disparo disparos[MAX_DISPAROS];
Enemigo enemigos[MAX_ENEMIGOS];

double tiempoUltimoDisparo = 0.5;
double tiempoDelay = 0.8; // Delay de 0.5 segundos entre disparos
int frameActual = 0; // Contador de fotogramas para la animación
int vidaJugador = 3;//////Variable para definir el numero de vida de a nave///////
bool juegoTerminado = false;////////////Variable para controlar el estado del juego///////////////////
bool juegoGanado = false; 

// Variables para las texturas
Textura texturaFondo;
Textura texturaPlayer;
Textura texturaEnemigo1;
Textura texturaEnemigo2;
Textura texturaCorazon;

// Variables del jugador
GLfloat posXPlayer = 0.0f;
GLfloat posYPlayer = -0.88f;
GLfloat escalaPlayer = 0.5f;
GLfloat velocidadPlayer = 0.1f;

// Prototipos de funciones
void invertirImagenVerticalmente(GLubyte* imageData, GLuint ancho, GLuint largo, GLuint bytesPerPixel);
bool cargarTGA(Textura* textura, const char* filename);
void display();
void initGL();
void teclaPresionada(unsigned char key, int x, int y);
void actualizarDisparos();
void actualizarDisparosEnemigos();///////////////////////////
void limpiarTexturas();
bool colision(GLfloat x1, GLfloat y1, GLfloat ancho1, GLfloat alto1, GLfloat x2, GLfloat y2, GLfloat ancho2, GLfloat alto2);

string obtenerDirectorioDelEjecutable();
void cambiarAlDirectorioDelEjecutable();


void actualizarEstadoJuego() {
    if (vidaJugador <= 0) {
        juegoTerminado = true;
    }

    bool todosEnemigosDerrotados = true;
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            todosEnemigosDerrotados = false;
            break;
        }
    }

    if (todosEnemigosDerrotados) {
        juegoGanado = true;
    }
}

void idle() {
    // Actualizar lógica del juego aquí
    actualizarEstadoJuego();

    // Redibujar la escena
    glutPostRedisplay();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Space Invaders");

    initGL();
    glutDisplayFunc(display);
    glutIdleFunc(idle);  // Actualizar continuamente
    
    glutKeyboardFunc(teclaPresionada);
    glutMainLoop();

    limpiarTexturas();  // Liberar texturas al salir

    return 0;
}

bool cargarTGA(Textura* textura, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: No se puede abrir el archivo TGA %s\n", filename);
        return false;
    }

    GLubyte header[18];
    fread(header, sizeof(GLubyte), 18, file);

    textura->ancho = header[12] + header[13] * 256;
    textura->largo = header[14] + header[15] * 256;
    textura->bpp = header[16];

    if (textura->bpp != 24 && textura->bpp != 32) {
        fclose(file);
        printf("Error: Solo se soportan imágenes TGA de 24 o 32 bits\n");
        return false;
    }

    GLuint bytesPerPixel = textura->bpp / 8;
    GLuint imageSize = textura->ancho * textura->largo * bytesPerPixel;
    textura->imageData = (GLubyte*)malloc(imageSize);

    if (textura->imageData == NULL) {
        fclose(file);
        printf("Error: No se puede asignar memoria para la imagen TGA\n");
        return false;
    }

    fread(textura->imageData, 1, imageSize, file);
    fclose(file);

    // Cambiar de BGR a RGB
    for (GLuint i = 0; i < imageSize; i += bytesPerPixel) {
        GLubyte temp = textura->imageData[i];
        textura->imageData[i] = textura->imageData[i + 2];
        textura->imageData[i + 2] = temp;
    }

    // Invertir la imagen verticalmente
    invertirImagenVerticalmente(textura->imageData, textura->ancho, textura->largo, bytesPerPixel);

    // Crear la textura en OpenGL
    glGenTextures(1, &textura->id);
    glBindTexture(GL_TEXTURE_2D, textura->id);
    glTexImage2D(GL_TEXTURE_2D, 0, (bytesPerPixel == 4) ? GL_RGBA : GL_RGB, textura->ancho, textura->largo, 0, (bytesPerPixel == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textura->imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (textura->imageData) {
        free(textura->imageData);
        textura->imageData = NULL;
    }
    return true;
}

void invertirImagenVerticalmente(GLubyte* imageData, GLuint ancho, GLuint largo, GLuint bytesPerPixel) {
    GLuint rowSize = ancho * bytesPerPixel;
    GLubyte* tempRow = (GLubyte*)malloc(rowSize);

    for (GLuint y = 0; y < largo / 2; y++) {
        GLubyte* row1 = imageData + y * rowSize;
        GLubyte* row2 = imageData + (largo - y - 1) * rowSize;

        memcpy(tempRow, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, tempRow, rowSize);
    }

    free(tempRow);
}

bool colision(GLfloat x1, GLfloat y1, GLfloat ancho1, GLfloat alto1, GLfloat x2, GLfloat y2, GLfloat ancho2, GLfloat alto2) {
    return x1 < x2 + ancho2 &&
           x1 + ancho1 > x2 &&
           y1 < y2 + alto2 &&
           y1 + alto1 > y2;
}

void actualizarDisparos() {
    for (int i = 0; i < MAX_DISPAROS; i++) {
        if (disparos[i].activo) {
            disparos[i].posY += 0.01f;  // Velocidad del disparo hacia arriba
            if (disparos[i].posY > 1.0f) {
                disparos[i].activo = false;  // Desactivar el disparo si sale de la pantalla
            } else {
                // Verificar colisiones con los enemigos
                for (int j = 0; j < MAX_ENEMIGOS; j++) {
                    if (enemigos[j].activo && colision(disparos[i].posX, disparos[i].posY, 0.02f, 0.1f, enemigos[j].posX - 0.2f, enemigos[j].posY - 0.2f, 0.4f, 0.4f)) {
                        enemigos[j].activo = false;  // Desactivar enemigo
                        disparos[i].activo = false;  // Desactivar disparo
                        puntos += 100;  // Sumar puntos por eliminar un enemigo
                        break;
                    }
                }
            }
        }
    }
}



/***** DISPAROS ENEMIGO ***////

double tiempoUltimoDisparoEnemigo = 0.0;
double tiempoDelayDisparoEnemigo = 2.0; // Intervalo de 2 segundos entre disparos de enemigos

void actualizarDisparosEnemigos() {
    double tiempoActual = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Obtener tiempo actual en segundos
    
    // Verificar si es tiempo de disparar
    if (tiempoActual - tiempoUltimoDisparoEnemigo > tiempoDelayDisparoEnemigo) {
        // Buscar un enemigo activo para disparar
        for (int i = 0; i < MAX_ENEMIGOS; i++) {
            if (enemigos[i].activo && (rand() % 100) < 20) { // 20% de probabilidad de disparar
                // Buscar un disparo inactivo para usar
                for (int j = 0; j < MAX_ENEMY_SHOTS; j++) {
                    if (!disparosEnemigos[j].activo) {
                        disparosEnemigos[j].activo = true;
                        disparosEnemigos[j].posX = enemigos[i].posX;
                        disparosEnemigos[j].posY = enemigos[i].posY; //- 0.2f; // Posición inicial del disparo del enemigo
                        disparosEnemigos[j].velocidadY = -0.02f; // Velocidad hacia abajo
                        break;
                    }
                }
                break; // Solo un enemigo dispara por intervalo
            }
        }
        
        tiempoUltimoDisparoEnemigo = tiempoActual; // Actualizar tiempo del último disparo
    }

    // Actualizar la posición de los disparos enemigos activos
    for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
        if (disparosEnemigos[i].activo) {
            disparosEnemigos[i].posY += disparosEnemigos[i].velocidadY;
            if (disparosEnemigos[i].posY < -1.0f) {
                disparosEnemigos[i].activo = false; // Desactivar disparo si sale de la pantalla
            } else if (colision(disparosEnemigos[i].posX, disparosEnemigos[i].posY, 0.02f, 0.1f, posXPlayer - 0.2f, posYPlayer - 0.2f, 0.4f, 0.4f)) {
                disparosEnemigos[i].activo = false; // Desactivar disparo
                puntos -= 200; // Restar puntos por ser alcanzado por un disparo enemigo
                if (puntos < 0) {
                    puntos = 0; // Asegurar que los puntos no sean negativos
                }
                vidaJugador--; // Reducir vida del jugador
                if (vidaJugador <= 0) {
                    juegoTerminado = true; // El juego ha terminado
                }
            }
        }
    }
}

void playAudio(const char* audioFilePath) {
    // Reproduce un archivo de audio utilizando WinMM
    PlaySound(audioFilePath, NULL, SND_ASYNC);
}

void mostrarMenu() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    const char* lineasMenu[] = {
        "GALAXIAN",
        "",
        "Presione Enter para comenzar",
        "Presione I para ver las Instrucciones",
        "Presione Esc para Salir"
    };
    int numLineas = sizeof(lineasMenu) / sizeof(lineasMenu[0]);
    float posY = 0.2f; // Posición inicial en Y

    for (int i = 0; i < numLineas; ++i) {
        glRasterPos2f(-0.2f, posY);
        const char* linea = lineasMenu[i];
        while (*linea) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *linea++);
        }
        posY -= 0.1f; // Ajuste para la siguiente línea
    }
}

void mostrarInstrucciones() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    const char* lineasInstrucciones[] = {
        "INSTRUCCIONES",
        "",
        "A/D: Mover izquierda y derecha",
        "Espacio: Disparar",
        "Esc: Volver al menu"
    };
    int numLineas = sizeof(lineasInstrucciones) / sizeof(lineasInstrucciones[0]);
    float posY = 0.0f; // Posición inicial en Y

    for (int i = 0; i < numLineas; ++i) {
        glRasterPos2f(-0.3f, posY);
        const char* linea = lineasInstrucciones[i];
        while (*linea) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *linea++);
        }
        posY -= 0.1f; // Ajuste para la siguiente línea
    }
}

  


///¡*************************** DISPLAY************************////
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    actualizarDisparos();  // Actualizar disparos del jugador
    actualizarDisparosEnemigos(); // Actualizar disparos de los enemigos

    // Dibujar fondo
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaFondo.id);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    // Dibujar puntos 
    
   if (puntos >= 0) {
        glColor3f(1.0f, 1.0f, 1.0f); // Color blanco para el puntaje
        glRasterPos2f(-0.95f, -0.7f); // Posición del puntaje en pantalla
        char buffer[50];
        sprintf(buffer, "Puntaje: %d", puntos); // Formatear el puntaje como una cadena de caracteres
        for (char* c = buffer; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Mostrar cada carácter del puntaje
        }
    }
   if (estadoActual == MENU) {
        mostrarMenu();
    }else if (estadoActual == INSTRUCCIONES) {
        mostrarInstrucciones();
	}else if (juegoGanado) {
         glColor3f(0.0f, 1.0f, 0.0f); // Color verde para el mensaje de "You Win!"
        glRasterPos2f(-0.1f, 0.0f);
        const char* mensaje = "You Win!";
        for (const char* c = mensaje; *c != '\0'; c++) {
            if (*c == '\n') {
                glRasterPos2f(-0.1f, -0.1f);
            } else {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
        }       
	}else if (juegoTerminado) {
        // Mostrar mensaje de "Game Over"
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.1f, 0.0f);
        const char* mensaje = "Game Over";
        for (const char* c = mensaje; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        
	}else if (juegoGanado) {
        glColor3f(0.0f, 1.0f, 0.0f); // Color verde para el mensaje de "You Win!"
        glRasterPos2f(-0.1f, 0.0f);
        const char* mensaje = "You Win!";
        for (const char* c = mensaje; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    } else {
    	// Dibujar jugador
  	  glEnable(GL_TEXTURE_2D);
  	  glBindTexture(GL_TEXTURE_2D, texturaPlayer.id);
  	  glColor3f(1.0f, 1.0f, 1.0f);
	  glPushMatrix();
  	  glTranslatef(posXPlayer, posYPlayer, 0.0f);
 	  glScalef(escalaPlayer * 0.4f, escalaPlayer * 0.4f, 1.0f);
  	  glBegin(GL_QUADS);
  	  glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
  	  glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
  	  glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
  	  glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
  	  glEnd();
  	  glPopMatrix();
  	  
   		glDisable(GL_TEXTURE_2D);
    
    	glColor3f(1.0f, 1.0f, 1.0f);  // Restaurar color a blanco
// Animación de enemigos
    frameActual = (frameActual + 1) % 500; // 60 fotogramas por segundo

   	 // Dibujar enemigos
   	 glEnable(GL_TEXTURE_2D);
  	  for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            if (frameActual < 30) {
                glBindTexture(GL_TEXTURE_2D, texturaEnemigo1.id);
            } else {
                glBindTexture(GL_TEXTURE_2D, texturaEnemigo2.id);
            }
            glPushMatrix();
            glTranslatef(enemigos[i].posX, enemigos[i].posY, 0.0f);
            glScalef(enemigos[i].escala, enemigos[i].escala, 1.0f);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.3f, -0.3f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(0.3f, -0.3f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(0.3f, 0.3f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.3f, 0.3f);
            glEnd();
            glPopMatrix();
      	  }
 	   }
 	   glDisable(GL_TEXTURE_2D);
 	     // Dibujar disparos del jugador
	    glColor3f(1.0f, 0.0f, 0.0f);  // Color rojo para los disparos
	    for (int i = 0; i < MAX_DISPAROS; i++) {
	        if (disparos[i].activo) {
	            glBegin(GL_QUADS);
	            glVertex2f(disparos[i].posX - 0.01f, disparos[i].posY - 0.02f);
	            glVertex2f(disparos[i].posX + 0.01f, disparos[i].posY - 0.02f);
	            glVertex2f(disparos[i].posX + 0.01f, disparos[i].posY + 0.02f);
	            glVertex2f(disparos[i].posX - 0.01f, disparos[i].posY + 0.02f);
	            glEnd();
	        }
	    }
	    glColor3f(1.0f, 1.0f, 1.0f);  // Restaurar color a blanco
	
	    // Dibujar disparos de los enemigos
	    glColor3f(0.0f, 1.0f, 0.0f);  // Color verde para los disparos de los enemigos
	    for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
	        if (disparosEnemigos[i].activo) {
	            glBegin(GL_QUADS);
	            glVertex2f(disparosEnemigos[i].posX - 0.01f, disparosEnemigos[i].posY - 0.02f);
	            glVertex2f(disparosEnemigos[i].posX + 0.01f, disparosEnemigos[i].posY - 0.02f);
	            glVertex2f(disparosEnemigos[i].posX + 0.01f, disparosEnemigos[i].posY + 0.02f);
	            glVertex2f(disparosEnemigos[i].posX - 0.01f, disparosEnemigos[i].posY + 0.02f);
	            glEnd();
	
	            // Desactivar el disparo si sale de la pantalla
	            if (disparosEnemigos[i].posY < -1.0f) {
	                disparosEnemigos[i].activo = false;
	            }
	        }
	    }
	    
	     // Dibujar corazones para representar las vidas del jugador
		    glEnable(GL_TEXTURE_2D);
		    glColor3f(1.0f, 0.0f, 0.0f); 
		    glBindTexture(GL_TEXTURE_2D, texturaCorazon.id);
		    glPushMatrix();
		    glTranslatef(-0.93f, 0.90f, 0.0f); // Ajustar posición al borde izquierdo
		
		    for (int i = 0; i < vidaJugador; ++i) {
		        glBegin(GL_QUADS);
		        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		        glTexCoord2f(1.0f, 0.0f); glVertex2f(0.1f, 0.0f);
		        glTexCoord2f(1.0f, 1.0f); glVertex2f(0.1f, 0.1f);
		        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.1f);
		        glEnd();
		        glTranslatef(0.12f, 0.0f, 0.0f); // Espaciado entre los corazones
		    }
		
		    glPopMatrix();
		    glDisable(GL_TEXTURE_2D);
		   }
    // Intercambiar buffers y forzar redibujado
    glutSwapBuffers();
    glutPostRedisplay();
}



void initGL() {
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f); // Color de fondo cyan
	cambiarAlDirectorioDelEjecutable();
    // Inicializar disparos d=e los enemigos
    for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
        disparosEnemigos[i].activo = false;

    // Inicializar disparos del jugador
    for (int i = 0; i < MAX_DISPAROS; i++) {
        disparos[i].activo = false;
    }
    
 // Inicializar enemigos
  // Inicializar enemigos
for (int i = 0; i < MAX_ENEMIGOS; i++) {
    enemigos[i].activo = true;
    enemigos[i].posX = (i % 5) * 0.4f - 0.8f;  // Ajustar posición inicial en x para estar dentro de la pantalla
    enemigos[i].posY = 0.8f - (i / 5) * 0.3f;  // Ajustar posición inicial en y para estar dentro de la pantalla
    enemigos[i].escala = 0.25f;  // Reducir escala a la mitad
    enemigos[i].velocidadX = 0.05f;  // Velocidad horizontal inicial
    enemigos[i].direccionX = 1.0f;   // Dirección inicial (1 para derecha)
}

    // Cargar texturas (asegúrate de que las rutas de los archivos sean correctas y existan)
    const char* fondoFilename = "tEs.tga";  // Ruta de la textura de fondo
    if (!cargarTGA(&texturaFondo, fondoFilename)) {
        printf("Error al cargar la textura de fondo.\n");
        exit(EXIT_FAILURE);
    }

    const char* playerFilename = "player.tga";  // Ruta de la textura del jugador
    if (!cargarTGA(&texturaPlayer, playerFilename)) {
        printf("Error al cargar la textura del jugador.\n");
        exit(EXIT_FAILURE);
    }

    const char* enemigo1Filename = "enemigo1.tga";  // Ruta de la textura del enemigo 1
    if (!cargarTGA(&texturaEnemigo1, enemigo1Filename)) {
        printf("Error al cargar la textura del enemigo 1.\n");
        exit(EXIT_FAILURE);
    }

    const char* enemigo2Filename = "enemigo2.tga";  // Ruta de la textura del enemigo 2
    if (!cargarTGA(&texturaEnemigo2, enemigo2Filename)) {
        printf("Error al cargar la textura del enemigo 2.\n");
        exit(EXIT_FAILURE);
    }
    
    const char* vidaCorazonFilename = "Corazon.tga";  // Ruta de la textura del corazon que indica el numero de vidas
    if (!cargarTGA(&texturaCorazon, vidaCorazonFilename)) {
        printf("Error al cargar la textura del enemigo 2.\n");
        exit(EXIT_FAILURE);
    }
    playAudio("Sonido.wav");
}


}

void teclaPresionada(unsigned char key, int x, int y) {
	 if (estadoActual == MENU) {
        switch (key) {
            case 13: // Enter
                estadoActual = JUGANDO;
                break;
            case 'i':
            case 'I':
                estadoActual = INSTRUCCIONES;
                break;
            case 27: // Esc
                exit(0);
        }
    } else if (estadoActual == JUGANDO) {
        if (juegoTerminado || juegoGanado) {
            return;
        }
	
        
   	 switch (key) {
    	case 27:
                estadoActual = MENU;
                break;
        case 'a':
        case 'A':
            posXPlayer -= velocidadPlayer;  // Mover a la izquierda
            if (posXPlayer < -0.95f) {
                    posXPlayer = -0.95f;
                }
            break;
        case 'd': 
		case 'D':       	
            posXPlayer += velocidadPlayer;  // Mover a la derecha
            if (posXPlayer > 0.95f) {
                    posXPlayer = 0.95f;
                }
            break;
            
        case ' ':
        	double tiempoActual = glutGet(GLUT_ELAPSED_TIME) / 10.0;
            if (tiempoActual - tiempoUltimoDisparo > tiempoDelay) {
			// Disparar
            for (int i = 0; i < MAX_DISPAROS; i++) {
                if (!disparos[i].activo) {
                    disparos[i].activo = true;
                    disparos[i].posX = posXPlayer;
                    disparos[i].posY = posYPlayer + 0.2f;  // Posición inicial del disparo un poco arriba del jugador
                     tiempoUltimoDisparo = tiempoActual;
                    break;
                }
            }
         }
            //break;
    	}
    }else if (estadoActual == INSTRUCCIONES) {
        if (key == 27) { // Esc
            estadoActual = MENU;
        }
    } else if (estadoActual == GANADO || estadoActual == PERDIDO) {
        if (key == 27) { // Esc
            estadoActual = MENU;
        }
    }
    // Limitar el movimiento del jugador dentro de ciertos límites (opcional)
    if (posXPlayer < -1.0f)
        posXPlayer = -1.0f;
    if (posXPlayer > 1.0f)
        posXPlayer = 1.0f;
    if (posYPlayer < -1.0f)
        posYPlayer = -1.0f;
    if (posYPlayer > 1.0f)
        posYPlayer = 1.0f;
    glutPostRedisplay();
}

void limpiarTexturas() {
    glDeleteTextures(1, &texturaFondo.id);
    glDeleteTextures(1, &texturaPlayer.id);
    glDeleteTextures(1, &texturaEnemigo1.id);
    glDeleteTextures(1, &texturaEnemigo2.id);
    glDeleteTextures(1, &texturaCorazon.id);
}

// Función para obtener el directorio del ejecutable
string obtenerDirectorioDelEjecutable() {
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL) {
        GetModuleFileName(hModule, path, sizeof(path));
        string fullPath(path);
        size_t pos = fullPath.find_last_of("\\/");
        return (string::npos == pos) ? "" : fullPath.substr(0, pos);
    }
    return "";
}

// Función para cambiar al directorio del ejecutable
void cambiarAlDirectorioDelEjecutable() {
    string dir = obtenerDirectorioDelEjecutable();
    if (!dir.empty()) {
        _chdir(dir.c_str());
    } else {
        cerr << "Error: No se pudo obtener el directorio del ejecutable." <<endl;
        exit(1);
    }
}
 
