#ifndef CARGA_ARCHIVO_H
#define CARGA_ARCHIVO_H

#include <vector>
#include <string>

using namespace std;

// Estructura para almacenar un vértice (x, y, z)
struct Vertice {
    float x, y, z;
};

// Estructura para almacenar una normal (nx, ny, nz)
struct Normal {
    float nx, ny, nz;
};

// Estructura para almacenar coordenadas de textura (u, v)
struct CoordenadaTextura {
    float u, v;
};

// Estructura para almacenar una cara
// Cada cara tiene 3 vértices, 3 coordenadas de textura y 3 normales
struct Cara {
    int v[3], t[3], n[3];
};

class Modelo {
public:
    vector<Vertice> vertices;
    vector<Normal> normals;
    vector<CoordenadaTextura> coordenadasTextura;
    vector<Cara> caras;
    float escala;

    Modelo(const char* filename, float escala);
    bool cargarOBJ(const char* filename, float escala);
};

#endif // CARGA_ARCHIVO_H

