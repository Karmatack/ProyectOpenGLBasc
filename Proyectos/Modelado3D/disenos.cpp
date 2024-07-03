#include <GL/glut.h>
#include <iostream>

using namespace std;

GLfloat angulo=0.0;

typedef struct {
    GLubyte *dibujo;
    GLuint bpp;
    GLuint largo;
    GLuint ancho;
    GLuint ID;
} textura;

textura cuadrado;
textura cuboCasa;
textura tpelota_multicolor;
textura cesped;
textura arbolTronco;
textura morada;

int cargaTGA(char const*nombre,textura *imagen){
	
	GLubyte cabeceraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte compararTGA[12];
	
	GLubyte cabecera[6];//características físicas de imagen-los 6 valores luego de los 12
	
	GLuint bytesporpuntos;
	GLuint tamanoimagen;
	GLuint temp,i;			
	GLuint tipo=GL_RGBA;
	
	FILE *archivo=fopen(nombre,"rb");  //el parametro de nombre asignado , leer en forma binaria-asignando a archivo
	
	if(archivo==NULL||
	fread(compararTGA,1,sizeof(compararTGA),archivo) !=sizeof(compararTGA)||//fread =file read , guardar en compararTGA, iterar de 1 en 1, los primeros 12, del archivo que acabo de crear
	memcmp(cabeceraTGA,compararTGA,sizeof(compararTGA))!=0||
	fread(cabecera,1,sizeof(cabecera),archivo)!= sizeof(cabecera)
	){     
			
		if(archivo==NULL){
		
			printf("Nos se encontró el archivo \n",nombre);
			return 0;
		}else{
		
			fclose(archivo);
			return 0;
		}
	}

	imagen->largo=256*cabecera[1] + cabecera[0]; //formula 256 que multiplica
	imagen->ancho=256*cabecera[3]+cabecera[2];//
	if(
		imagen->largo<=0||
		imagen->ancho<=0||
		(cabecera[4]!=24&&cabecera[4]!=32)//RGB - RGBA
	){
		printf("Datos inválidos\n");
		fclose(archivo);
		return 0;
	}
	imagen->bpp=cabecera[4];
	bytesporpuntos= cabecera[4]/8;//cantidad de bits por punto
	tamanoimagen= imagen->largo*imagen->ancho * bytesporpuntos;  //el peso del archivo
	//calculo  / 1024
	
	imagen->dibujo=(GLubyte *)malloc(tamanoimagen);
	if(
		imagen->dibujo==NULL||
		fread(imagen->dibujo,1,tamanoimagen,archivo)!=tamanoimagen
	)
	{
		if(imagen->dibujo!=NULL){
			printf("Error leyendo imagen\n");
			free(imagen->dibujo); //liberar memoria luego de terminar el proceso
		}else{
			printf("Error asignando memoria\n");
		}
		fclose(archivo);
		return 0;
	}
	//TGA : BRGA	RGBA
	for(i=0;i<(int)tamanoimagen;i+=bytesporpuntos){
		temp=imagen->dibujo[i];					// a-->temporal 
		imagen->dibujo[i]=imagen->dibujo[i+2]; 	// b-->a
		imagen->dibujo[i+2]=temp;				//temp-->b
	
	}
	fclose(archivo);
	
	glGenTextures(1,&imagen[0].ID);
	glBindTexture(GL_TEXTURE_2D,imagen[0].ID);
	if(imagen->bpp==24) tipo=GL_RGB;
	glTexImage2D(GL_TEXTURE_2D,0,tipo,imagen[0].ancho,imagen[0].largo,0,tipo,GL_UNSIGNED_BYTE,imagen[0].dibujo);
	//para reducir o ampliar
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return 1;
	
}

void inicializarTextura() {
    if (!cargaTGA("tga\\espacio.tga", &cuadrado)) {
        printf("Error cargando textura cuadrado \n");
        exit(0);
    }
    else if (!cargaTGA("tga\\pared.tga", &cuboCasa)) {
        printf("Error cargando textura cuboCasa \n");
        exit(0);
    }
    else if(!cargaTGA("tga\\multicolorpelota.tga",&tpelota_multicolor)){
		printf("Error cargando textura multicolorpelota \n");
		exit(0);
	}
	else if(!cargaTGA("tga\\cesped.tga",&cesped)){
		printf("Error cargando textura cesped \n");
		exit(0);
	}
	else if(!cargaTGA("tga\\tronco_arbol.tga",&arbolTronco)){
		printf("Error cargando textura arbolTronco \n");
		exit(0);
	}
	else if(!cargaTGA("tga\\Morado.tga",&morada)){
		printf("Error cargando textura morada \n");
		exit(0);
	}	
	
}

void cuadradoEspacio(int tamano) {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, cuadrado.ID);
    
    int c = tamano / 2;
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3d(-c, c, c);
    glTexCoord2f(1.0, 0.0); glVertex3d(c, c, c);
    glTexCoord2f(1.0, 1.0); glVertex3d(c, -c, c);
    glTexCoord2f(0.0, 1.0); glVertex3d(-c, -c, c);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void cubo_Casa(int tamano) {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
	glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, cuboCasa.ID);
    
    int c = tamano / 2; // Dividir por 2 para centrar el cubo en el origen
    
    glBegin(GL_QUADS);
    // Lado frontal
    glTexCoord2f(0.0, 0.0); glVertex3d(-c, c, c);
    glTexCoord2f(1.0, 0.0); glVertex3d(c, c, c);
    glTexCoord2f(1.0, 1.0); glVertex3d(c, -c, c);
    glTexCoord2f(0.0, 1.0); glVertex3d(-c, -c, c);
    // Lado derecho
    glTexCoord2f(0.0, 0.0); glVertex3d(c, c, c);
    glTexCoord2f(1.0, 0.0); glVertex3d(c, c, -c);
    glTexCoord2f(1.0, 1.0); glVertex3d(c, -c, -c);
    glTexCoord2f(0.0, 1.0); glVertex3d(c, -c, c);
    // Lado posterior
    glTexCoord2f(0.0, 0.0); glVertex3d(c, c, -c);
    glTexCoord2f(1.0, 0.0); glVertex3d(-c, c, -c);
    glTexCoord2f(1.0, 1.0); glVertex3d(-c, -c, -c);
    glTexCoord2f(0.0, 1.0); glVertex3d(c, -c, -c);
    // Lado izquierdo
    glTexCoord2f(0.0, 0.0); glVertex3d(-c, c, -c);
    glTexCoord2f(1.0, 0.0); glVertex3d(-c, c, c);
    glTexCoord2f(1.0, 1.0); glVertex3d(-c, -c, c);
    glTexCoord2f(0.0, 1.0); glVertex3d(-c, -c, -c);
    // Lado superior
    glTexCoord2f(0.0, 0.0); glVertex3d(-c, c, -c);
    glTexCoord2f(1.0, 0.0); glVertex3d(c, c, -c);
    glTexCoord2f(1.0, 1.0); glVertex3d(c, c, c);
    glTexCoord2f(0.0, 1.0); glVertex3d(-c, c, c);
    // Lado inferior
    glTexCoord2f(0.0, 0.0); glVertex3d(-c, -c, c);
    glTexCoord2f(1.0, 0.0); glVertex3d(c, -c, c);
    glTexCoord2f(1.0, 1.0); glVertex3d(c, -c, -c);
    glTexCoord2f(0.0, 1.0); glVertex3d(-c, -c, -c);
    glEnd();
    
	glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void pelota(int tam){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	
	angulo += 0.5;
	
	glRotatef(angulo,1,1,1);

	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D,tpelota_multicolor.ID);
	gluSphere(qobj,tam,50,50);
	gluDeleteQuadric(qobj);  
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);	
	glEnable(GL_LIGHTING); 
	glPopMatrix();
}

void pelotaMorada(int tam){
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	angulo += 0.5;
	
	glRotatef(angulo,1,1,1);

	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D,morada.ID);
	gluSphere(qobj,tam,50,50);
	gluDeleteQuadric(qobj);  
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);	
	glEnable(GL_LIGHTING); 
	glPopMatrix();
}

void cuadradocesped(int tamano) {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, cesped.ID);
    
    int c = tamano / 2;
    
    glRotatef(90,1,0,0);
		glTranslatef(0,0,-4800);
		    glBegin(GL_QUADS);
		    glTexCoord2f(0.0, 0.0); glVertex3d(-c, c, c);
		    glTexCoord2f(1.0, 0.0); glVertex3d(c, c, c);
		    glTexCoord2f(1.0, 1.0); glVertex3d(c, -c, c);
		    glTexCoord2f(0.0, 1.0); glVertex3d(-c, -c, c);
		    glEnd();
	    glTranslatef(0,0,4800);
    glRotatef(-90,1,0,0);
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void tronco(float ancho, float alto) {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D, arbolTronco.ID);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    gluCylinder(quad, ancho / 2.0, ancho / 2.0, alto, 30, 15);

    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}



