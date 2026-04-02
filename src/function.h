#ifndef FUNCTION_H
#define FUNCTION_H

#include "utils.h"
#include "mesh.h"
#include "function.h"
#include <vector>

//valeurs globales :
//les bornes des intervalles de x et y
extern float x_min, x_max, y_min, y_max, z_min, z_max;
//variable de récup pour la fenêtre ImgUI (saisie de texte, string)
extern char xminIB[256]; extern char xmaxIB[256]; extern char yminIB[256]; extern char ymaxIB[256], zminIB[256], zmaxIB[256];
//Variale qui tient compte d'une redéfinition :
extern bool edited;

//Fonction de R2 dans R (surface) d'essai:
float f(float x, float y);

//on refait les ensemble de définitions de 0 à n en de min à max pour x et y (rerange([0, n])= [min, max])
float rerangex(float x, float x_min, float x_max);
float rerangey(float y, float y_min, float y_max);

//Fonction à effet de bord sur le monoïde Surf (dans mesh.h/mesh.cpp) : calcule les vertices
void calculate(int nbX = 100, int nbY = 100);

#endif