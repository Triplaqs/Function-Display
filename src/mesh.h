#ifndef MESH_H
#define MESH_H

#include "mesh.h"
#include <vector>

//Structure Pour surface de fonction
typedef struct Surf{
    int nX = 100;
    int nY = 100;
    int size(){
        return nX*nY;
    }
    std::vector<float> vertices; 
    std::vector<unsigned int> triangles;
    std::vector<float> normals;
    std::vector<float> colors;
} Surf;

extern Surf surf;

#endif