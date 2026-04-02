#include "utils.h"
#include "mesh.h"
#include "function.h"
#include <vector>

//variables globales :
float x_min = -15.0f, x_max = 15.0f, y_min = -15.0f, y_max = 15.0f, z_min = 15.0f, z_max = -15.0f;
//variable de récup pour la fenêtre ImgUI (saisie de texte, string)
char xminIB[256] = "-15.0"; char xmaxIB[256] = "15.0"; char yminIB[256] = "-15.0"; char ymaxIB[256] = "15.0"; char zminIB[256] = "15.0"; char zmaxIB[256] = "-15.0";
//Variale qui tient compte d'une redéfinition :
bool edited = false;

///////////////////////////////////////////// FUNCTION ///////////////////////////////////////////////// //voir si std::function<> peut apporter ?
//Fonction de R2 dans R (surface) d'essai:
float f(float x, float y) {
    if((x*x<=0.2f || y*y<=0.2f) && (x*y >= 0)){
        return z_max;
    } else if((x*x<=0.2f || y*y<=0.2f) && (x*y <= 0)){
        return z_min;
    } else{
        return (x + y)/(x*y);
    } 
}



//transforme [0, nY] en [y_min, y_max] où miny et maxy globaux
float rerangex(float x) {
    //return x_min + (x - 0.0f) * (x_max - x_min) / (1.0f - 0.0f);
    return x_min + x * (x_max - x_min);
}
//transforme [0, nX] en [x_min, x_max] où minx et maxx globaux
float rerangey(float y) {
    //return y_min + (y - 0.0f) * (y_max - y_min) / (1.0f - 0.0f);
    return y_min + y * (y_max - y_min);
}

//Fonction à effet de bord sur le monoïde Surf (dans mesh.h/mesh.cpp) : calcule les vertices
void calculate(int nbX, int nbY){
    surf.clear();
    surf.nX = nbX;
    surf.nY = nbY;
    for (int i = 0; i < nbX; ++i) {
        for (int j = 0; j < nbY; ++j) {
            float x = rerangex((float)i / (nbX - 1));
            float y = rerangey((float)j / (nbY - 1));
            //float x = rerangex((float)i);
            //float y = rerangey((float)j);
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
    printf("calculate : x : %f\n", surf.vertices[0]);
    printf("calculate : y : %f\n", surf.vertices[1]);
    printf("calculate : z : %f\n", surf.vertices[2]);
    printf("calculate : x_min : %f\n", x_min);
    printf("calculate : y_min : %f\n", y_min);
    printf("calculate : x_max : %f\n", x_max);
    printf("calculate : y_max : %f\n\n", y_max);
    for(int i = 0; i < nbX - 1; ++i) {
        for(int j = 0; j < nbY - 1; ++j) {
            surf.triangles.push_back(i * nbY + j);
            surf.triangles.push_back((i + 1) * nbY + j);
            surf.triangles.push_back((i + 1) * nbY + (j + 1));
            surf.triangles.push_back(i * nbY + j);
            surf.triangles.push_back((i + 1) * nbY + (j + 1));
            surf.triangles.push_back(i * nbY + (j + 1));
        }
    }
    
}

//pour les dérivés (surf.normals) on a une piste ici :
//https://stackoverflow.com/questions/1559695/implementing-the-derivative-in-c-c


//calculate gestion des points non définis