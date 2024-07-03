#include <GL/glut.h>
#include <iostream>

using namespace std;


void configurarLucesYMateriales(GLfloat light_position[4], GLfloat light_diffuse[4], GLfloat mat_diffuse[4]) {
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    GLfloat light_ambient[] = {0, 0, 0, 1};
    GLfloat light_specular[] = {1, 1, 1, 1};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    GLfloat mat_ambient[] = {0, 0, 0, 1};
    GLfloat mat_specular[] = {1, 1, 1, 1};
    GLfloat mat_shininess[] = {100};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void configurarColorMaterial(GLfloat mat_diffuse[4], const string& color) {
    if (color == "rojo") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "naranja") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0.5; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "amarillo") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 1; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "verde") {
        mat_diffuse[0] = 0; mat_diffuse[1] = 1; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "azul") {
        mat_diffuse[0] = 0; mat_diffuse[1] = 0; mat_diffuse[2] = 1; mat_diffuse[3] = 1;
    } else if (color == "violeta") {
        mat_diffuse[0] = 0.5; mat_diffuse[1] = 0; mat_diffuse[2] = 0.5; mat_diffuse[3] = 1;
    } else if (color == "blanco") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 1; mat_diffuse[2] = 1; mat_diffuse[3] = 1;
    } else if (color == "negro") {
        mat_diffuse[0] = 0; mat_diffuse[1] = 0; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "rosa") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0.75; mat_diffuse[2] = 0.8; mat_diffuse[3] = 1;
    } else if (color == "gris") {
        mat_diffuse[0] = 0.5; mat_diffuse[1] = 0.5; mat_diffuse[2] = 0.5; mat_diffuse[3] = 1;
    } else if (color == "marron") {
        mat_diffuse[0] = 0.65; mat_diffuse[1] = 0.16; mat_diffuse[2] = 0.16; mat_diffuse[3] = 1;
    } else if (color == "cyan") {
        mat_diffuse[0] = 0; mat_diffuse[1] = 1; mat_diffuse[2] = 1; mat_diffuse[3] = 1;
    } else if (color == "magenta") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0; mat_diffuse[2] = 1; mat_diffuse[3] = 1;
    } else if (color == "turquesa") {
        mat_diffuse[0] = 0.25; mat_diffuse[1] = 0.88; mat_diffuse[2] = 0.82; mat_diffuse[3] = 1;
    } else if (color == "dorado") {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0.84; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    } else if (color == "plateado") {
        mat_diffuse[0] = 0.75; mat_diffuse[1] = 0.75; mat_diffuse[2] = 0.75; mat_diffuse[3] = 1;
    } else if (color == "rosa") {
        mat_diffuse[0] = 1.0;  mat_diffuse[1] = 0.75; mat_diffuse[2] = 0.8;  mat_diffuse[3] = 1.0;  
    }else {
        mat_diffuse[0] = 1; mat_diffuse[1] = 0; mat_diffuse[2] = 0; mat_diffuse[3] = 1;
    }
}

void luna(int tamano){
	
	GLfloat light_position[] = {-0.5,1,0,0};	//posición de la luz
	GLfloat light_diffuse[] = {1,1,1,1};		
	GLfloat mat_diffuse[]={1,1,0,1};			//ambiente color
		
	configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
	glutSolidSphere(tamano, tamano*0.4, tamano*0.4); //(radius, slices, stacks)
}

void flotador(int tam, string color) {
    glPushMatrix();
    
    GLfloat light_position[] = {1, 1, 0, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};  
    GLfloat mat_diffuse[4];
    
	configurarColorMaterial(mat_diffuse, color);

    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    glutSolidTorus(tam*0.4, tam, 150, 150);
    
    glPopMatrix();
}


void ulaula(int tam, string color){
	glPushMatrix();

	GLfloat light_position[] = {1,1,0,0};
    GLfloat light_diffuse[] = {1,1,1,1}; 
	GLfloat mat_diffuse[4];
	
	configurarColorMaterial(mat_diffuse, color);
    
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
	glutSolidTorus(tam*0.1,tam,150,150);
	glPopMatrix();
}
void cilindro(float tam, const string& color) {
    glPushMatrix();
    GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat mat_diffuse[4];
    
    configurarColorMaterial(mat_diffuse, color);
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	    GLUquadric* quad = gluNewQuadric();
	    gluCylinder(quad, 0.5 * tam, 0.5 * tam, 2.0 * tam, 30 * tam, 15 * tam);
	    gluDeleteQuadric(quad);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    
    
    glPopMatrix();
}

void esfera(float tam, const string& color) {
    glPushMatrix();
    GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat mat_diffuse[4];
    
    
    configurarColorMaterial(mat_diffuse, color);
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    
    glutSolidSphere(tam, 80,80);
    
    glPopMatrix();
}

void tetera(float tam, const string& color) {
    glPushMatrix();
    GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat mat_diffuse[4];
    
    configurarColorMaterial(mat_diffuse, color);
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    
    glutSolidTeapot(tam);
    
    glPopMatrix();
}
void dodecaedro(float tam, const string& color) {
    glPushMatrix();
    GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat mat_diffuse[4];
    
    configurarColorMaterial(mat_diffuse, color);
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    
    glScalef(tam, tam, tam);
    glutSolidDodecahedron();
    
    glPopMatrix();
}
void octaedro(float tam, const string& color) {
    glPushMatrix();
    GLfloat light_position[] = {1, 1, 1, 0};
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    GLfloat mat_diffuse[4];
    
    configurarColorMaterial(mat_diffuse, color);
    configurarLucesYMateriales(light_position, light_diffuse, mat_diffuse);
    
    glScalef(tam, tam, tam);
    glutSolidOctahedron();
    
    glPopMatrix();
}



