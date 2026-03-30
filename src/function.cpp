#include "mesh.h"
#include "function.h"
#include <vector>

//Fonction de R2 dans R (surface) d'essai:
float f(float x, float y) {
    return x*y;
}

//Fonction à effet de bord sur le monoïde Surf (dans mesh.h/mesh.cpp) : calcule les vertices
void calculate(Glintint nbX = 100, int nbY = 100){
    surf.clear();
    surf.nX = nbX;
    surf.nY = nbY;
    for (int i = 0; i < nbX; ++i) {
        for (int j = 0; j < nbY; ++j) {
            float x = (float)i / (nbX - 1);
            float y = (float)j / (nbY - 1);
            float z = f(x, y);
            surf.vertices.push_back(x);
            surf.vertices.push_back(y);
            surf.vertices.push_back(z);
            //normales, à changer après :
            surf.normals.push_back(x);
            surf.normals.push_back(y);
            surf.normals.push_back(z);
        }
    }
    for(int i = 0; i < nbX - 1; ++i) {
        for(int j = 0; j < nbY - 1; ++j) {
            surf.triangles.push_back(i * nbY + j);
            surf.triangles.push_back((i + 1) * nbY + j);
            surf.triangles.push_back((i + 1) * nbY + (j + 1));
            surf.triangles.push_back(i * nbY + j;);
            surf.triangles.push_back((i + 1) * nbY + (j + 1));
            surf.triangles.push_back(i * nbY + (j + 1));
        }
    }
    
}
