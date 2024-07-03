#include <GL/glut.h>
#include <iostream>
#include "carga_Archivo.h"
#include "camara.h"
#include "figuras.h"
#include "disenos.h"
#include <direct.h>

using namespace std;

/*
total_tiempo_programando_aquí= 1d 11h 15m

Querido programador:

Cuando escribí este código, sólo Dios, chatGPT y yo sabíamos cómo funcionaba.
¡Ahora, sólo Dios lo sabe, chatGPT se equivoca!

Así que si está tratando de 'optimizar' esta rutina y fracasa (seguramente),
por favor, incremente el siguiente contador como una advertencia
para el siguiente colega:

total_horas_perdidas_aquí_debugeando = 9
hoy
debugeo comenzó		: 13:25
debugeo finalicé	: 15:30
*/


// Vector de modelos
vector<Modelo> modelos;

// Variables para la posición del avion y carro
GLfloat avionX = 0.0f, avionY = 0.0f, avionZ = 0.0f;
GLfloat carroX = 0.0f, carroY = 0.0f, carroZ = 0.0f;

void init();
void idle();
void modelar(int posicion);
void reshape(int largo, int ancho);
void manejarTeclado(unsigned char key, int x, int y);
void mouse(int boton, int estado, int x, int y);
void mouseMotion(int x, int y);
string obtenerDirectorioDelEjecutable();
void cambiarAlDirectorioDelEjecutable();

void arbol(const char* color = "verde");
void trasladarArbol(float traslacionX, float traslacionY, float traslacionZ, const char* color = "verde");
void cercaArbol();
void espacioFeo();
void muro();
void cercaMuro();


void display() {
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    actualizarCamara(); // Llamar a la función de la cámara desde camara.h
    
    //Base
	cuadradocesped(10000);
	//Bosque de Árboles
	cercaArbol();
    //Cerca de Muro
    cercaMuro();
    //Modelos 3d Figuritas
    glTranslatef(-200,-200,0);		//carro
		modelar(0);
	glTranslatef(200,0,0); 
	
	glTranslatef(2000,0,-1000);			//oso
		modelar(1);
	glTranslatef(1000,0,0);
		modelar(3);
	glTranslatef(-3000,0,1000);
	
	glTranslatef(600,0,0);			//avión
		glRotatef(270,0,1,0);
			modelar(2);
		glRotatef(-270,0,1,0);
	glTranslatef(-600,200,0);
	
	//Luna
	glTranslatef(-5000,3000,-6000);
		luna(600);
	glTranslatef(5000,-3000,6000);
	//Pelota de colores
	glTranslatef(3000,0,-2000);
		pelota(200);
	glTranslatef(-3000,0,2000);
	//Pelota Morada
	glTranslatef(-2000,0,-2000);
		pelotaMorada(200);
	glTranslatef(2000,0,2000);
	//Flotadores
	glTranslatef(0,3000,4500);
		flotador(1500,"rojo");
	glTranslatef(0,-3000,-4500);
	
	glTranslatef(3800,0,-3000);
		flotador(100,"violeta");
	glTranslatef(-1000,0,0);
		flotador(100,"naranja");
	glTranslatef(-1000,0,0);
		flotador(100,"amarillo");
	glTranslatef(-1000,0,0);
		flotador(100,"verde");
	glTranslatef(-1000,0,0);
		flotador(100,"azul");
	glTranslatef(-1000,0,0);
		flotador(100,"blanco");
	glTranslatef(-1000,0,0);
		flotador(100,"negro");
	glTranslatef(-1000,0,0);
		flotador(100,"cyan");
	glTranslatef(3200,0,3000);	
	//cilindro
	glTranslatef(3200,500,3000);
		cilindro(400.0,"verde");
	glTranslatef(-3200,-500,-3000);
	//Tetera
	glTranslatef(-3200,0,3000);
		tetera(100.0, "turquesa");
	glTranslatef(3200,0,-3000);
	//Dodecaedro
	glTranslatef(-2200,0,3000);
		dodecaedro(100.0, "rojo");
	glTranslatef(2200,0,-3000);
	//octaedro
	glTranslatef(-1200,0,3000);
		octaedro(100.0, "naranja");
	glTranslatef(1200,0,-3000);
	
	//pelota(8);
	glTranslatef(-200,0,3000);
		pelota(80);
	glTranslatef(200,0,-3000);
	
	//flotador(100,"violeta"); 		// colores: rojo - naranja - amarillo - verde - azul - violeta - blanco - negro - rosa - gris - marron - cyan - magenta - turquesa - dorado - plateado - rosa
	//ulaula(200,"gris");			// colores: rojo - naranja - amarillo - verde - azul - violeta - blanco - negro - rosa - gris - marron - cyan - magenta - turquesa - dorado - plateado - rosa
	//cilindro(50.0, "verde");		// colores: rojo - naranja - amarillo - verde - azul - violeta - blanco - negro - rosa - gris - marron - cyan - magenta - turquesa - dorado - plateado - rosa
	//tetera(80.0, "turquesa");
	//esfera(80.0, "rojo");
	//dodecaedro(20.0, "rojo");
	//octaedro(20.0, "rojo");
	//pelota(8);
	//pelotaMorada();
	//espacioFeo();
	
	

	
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(1200, 200);
    glutCreateWindow("Proyecto Final/ Juego 1");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(manejarTeclado); // Registrar la función de teclado
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

void init() {
		
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	cambiarAlDirectorioDelEjecutable();
	inicializarTextura();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	
    // Cargar los archivos .obj con la escala deseada
    modelos.push_back(Modelo("obj\\carro.obj", 1.0f));
    modelos.push_back(Modelo("obj\\BearSaddle.obj", 8.0f)); // Añade más modelos con diferentes escalas
	modelos.push_back(Modelo("obj\\BattleSpaceship.obj", 8.0f));
	modelos.push_back(Modelo("obj\\Horse.obj", 8.0f));
	
}

void modelar(int posicion){		//modela a mis figuras

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
        glPushMatrix();

        if (posicion == 2) { 		// Si es el carro, aplicar las transformaciones
            glTranslatef(avionX, avionY, avionZ);
        }
        else if(posicion == 0){		// Si es el oso, aplicar las transformaciones
			glTranslatef(carroX, carroY, carroZ);
		}
		glBindTexture(GL_TEXTURE_2D,morada.ID);
        glScalef(modelos[posicion].escala, modelos[posicion].escala, modelos[posicion].escala);

		glBegin(GL_TRIANGLES);

        for (const Cara& cara : modelos[posicion].caras) {
            // Generar colores aleatorios para cada triángulo
            /*glColor3f(static_cast<float>(rand()) / RAND_MAX,
                      static_cast<float>(rand()) / RAND_MAX,
                      static_cast<float>(rand()) / RAND_MAX);*/
            for (int j = 0; j < 3; j++) {
                if (!modelos[posicion].coordenadasTextura.empty()) {
                    CoordenadaTextura coordTextura = modelos[posicion].coordenadasTextura[cara.t[posicion] - 1];
                    glTexCoord2f(coordTextura.u, coordTextura.v);
                }
                if (!modelos[posicion].normals.empty()) {
                    Normal normal = modelos[posicion].normals[cara.n[posicion] - 1];
                    glNormal3f(normal.nx, normal.ny, normal.nz);
                }
                Vertice vertice = modelos[posicion].vertices[cara.v[j] - 1];
                glVertex3f(vertice.x, vertice.y, vertice.z);
            }
        }
        glEnd();
		glDisable(GL_TEXTURE_2D);
        glPopMatrix();
}

void idle() {
    glutPostRedisplay();
}

void reshape(int largo, int ancho) {
    glViewport(0, 0, (GLsizei)largo, (GLsizei)ancho);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ortoIzquierda, ortoDerecha, ortoAbajo, ortoArriba, ortoCerca, ortoLejos);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int boton, int estado, int x, int y) {
    manejarMouse(boton, estado, x, y); // Llamar a la función de mouse desde camara.h
}

void mouseMotion(int x, int y) {
    manejarMovimientoMouse(x, y); // Llamar a la función de movimiento de mouse desde camara.h
}

void manejarTeclado(unsigned char key, int x, int y) {
    const GLfloat desplazamiento = 20.0f;
    switch (key) {
        case 's':
            avionX -= (desplazamiento*3);
            break;
        case 'w':
            avionX += (desplazamiento*3);
            break;
        case 'd':
            avionZ += (desplazamiento*3);
            break;
        case 'a':
            avionZ -= (desplazamiento*3);
            break;
        case 'q':
            avionY -= (desplazamiento*3);
            break;
        case 'e':
            avionY += (desplazamiento*3);
            break;
//////////////////////////////////////////////////////////////////////////////////////////
        case 'l':
            carroX -= desplazamiento;
            break;
        case 'j':
            carroX += desplazamiento;
            break;
        case 'k':
           	carroZ -= desplazamiento;
            break;
        case 'i':
            carroZ += desplazamiento;
            break;    

        default:
            break;
    }
    glutPostRedisplay(); // Redibujar la escena
}


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

void arbol(const char* color ) {
	
	glTranslatef(0,250,0);
		
	    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	    	tronco(100.0, 500.0);
	    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	
	    const char* colorSeleccionado = color;
	    if (strcmp(color, "naranja") != 0 && strcmp(color, "rosa") != 0) {
	        colorSeleccionado = "verde";
	    }
	
	    esfera(150.0, colorSeleccionado);
	    
	    glTranslatef(100, 0, 0);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(-100, 0, 0);
	    
	    glTranslatef(-100, 0, 0);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(100, 0, 0);
	    
	    glTranslatef(0, -100, 0);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(0, 100, 0);
	    
	    glTranslatef(0, 100, 0);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(0, -100, 0);
	    
	    glTranslatef(0, 0, -100);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(0, 0, 100);
	    
	    glTranslatef(0, 0, 100);
	    	esfera(150.0, colorSeleccionado);
	    glTranslatef(0, 0, -100);
	    
    glTranslatef(0,-250,0);
}

void trasladarArbol(float traslacionX, float traslacionY, float traslacionZ, const char* color) {
    glTranslatef(traslacionX, traslacionY, traslacionZ);
    arbol(color);
    glTranslatef(-traslacionX, -traslacionY, -traslacionZ);
}

void cercaArbol(){
	trasladarArbol(-4800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(-3800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(-2800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(-1800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(-800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(0,0,-4800,"naranja"); //verde - rosa - naranja
	trasladarArbol(800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(1800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(2800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(3800,0,-4800,"verde"); //verde - rosa - naranja
	trasladarArbol(4800,0,-4800,"verde"); //verde - rosa - naranja
	
	trasladarArbol(-4800,0,-3800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,-2800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,-1800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,-800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,-0,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,1800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,2800,"rosa"); //verde - rosa - naranja
	trasladarArbol(-4800,0,3800,"rosa"); //verde - rosa - naranja
	
	trasladarArbol(4800,0,-3800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,-2800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,-1800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,-800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,-0,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,1800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,2800,"rosa"); //verde - rosa - naranja
	trasladarArbol(4800,0,3800,"rosa"); //verde - rosa - naranja
}

void muro(){
	glTranslatef(0,100,0);
		cubo_Casa(600);
	glTranslatef(0,-100,0);
	
	glTranslatef(0,700,0);
		cubo_Casa(600);
	glTranslatef(0,-700,0);
	
	glTranslatef(600,100,0);
		cubo_Casa(600);
	glTranslatef(-600,-100,0);
	
	glTranslatef(600,700,0);
		cubo_Casa(600);
	glTranslatef(-600,-700,0);
}

void cercaMuro(){
	glTranslatef(-4500,0,4800);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(1200,0,0);
		muro();
	glTranslatef(-3900,0,-4800);
}

void espacioFeo(){
	glTranslatef(0,0,-20000);
	cuadradoEspacio(20000);
	glTranslatef(0,0,20000);
	
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	glTranslatef(0,0,-20000);
	cuadradoEspacio(20000);
	glTranslatef(0,0,20000);
	glRotatef(-90, 0.0f, 1.0f, 0.0f);
	
	glRotatef(-90, 0.0f, 1.0f, 0.0f);
	glTranslatef(0,0,-20000);
	cuadradoEspacio(20000);
	glTranslatef(0,0,20000);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glTranslatef(0,0,-20000);
	cuadradoEspacio(20000);
	glTranslatef(0,0,20000);
	glRotatef(90, 1.0f, 0.0f, 0.0f);

}
//Karmatack



