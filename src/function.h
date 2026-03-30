#ifndef FUNCTION_H
#define FUNCTION_H

#include "mesh.h"
#include "function.h"
#include <vector>

//Fonction de R2 dans R (surface) d'essai:
float f(float x, float y);

//Fonction à effet de bord sur le monoïde Surf (dans mesh.h/mesh.cpp) : calcule les vertices
void calculate(int nbX = 100, int nbY = 100);

#endif