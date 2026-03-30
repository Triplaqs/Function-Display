#ifndef UTILS_H
#define UTILS_H

//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//headers
#include "utils.h"

//variables globales :
//variables pour utilisation du temps
extern float startTime;
extern float cooldown; //temps fixé de cooldown entre chaque action
extern float t_press; // Temps écoulé depuis la dernière pression

#endif