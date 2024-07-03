#include "carga_Archivo.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Modelo::Modelo(const char* filename, float escala) {
    this->escala = escala;
    cargarOBJ(filename, escala);
}

bool Modelo::cargarOBJ(const char* filename, float escala) {
    // Limpiar los vectores antes de cargar un nuevo archivo
    vertices.clear();
    normals.clear();
    coordenadasTextura.clear();
    caras.clear();

    // Abrir el archivo .obj
    ifstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return false;
    }

    string linea;
    // Leer el archivo línea por línea
    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string prefijo;
        iss >> prefijo;
        if (prefijo == "v") {
            // Leer un vértice
            Vertice vertice;
            iss >> vertice.x >> vertice.y >> vertice.z;
            // Aplicar la escala al vértice
            vertice.x *= escala;
            vertice.y *= escala;
            vertice.z *= escala;
            vertices.push_back(vertice);
        } else if (prefijo == "vn") {
            // Leer una normal
            Normal normal;
            iss >> normal.nx >> normal.ny >> normal.nz;
            normals.push_back(normal);
        } else if (prefijo == "vt") {
            // Leer una coordenada de textura
            CoordenadaTextura coordTextura;
            iss >> coordTextura.u >> coordTextura.v;
            coordenadasTextura.push_back(coordTextura);
        } else if (prefijo == "f") {
            // Leer una cara
            Cara cara;
            char barra;
            for (int i = 0; i < 3; i++) {
                // Leer índices de vértice, coordenada de textura y normal
                iss >> cara.v[i] >> barra >> cara.t[i] >> barra >> cara.n[i];
            }
            caras.push_back(cara);
        } else if (prefijo == "s") {
            // Manejar grupos de suavizado si es necesario
            string grupoSuavizado;
            iss >> grupoSuavizado;
        }
    }

    archivo.close();
    return true;
}

