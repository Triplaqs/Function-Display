//includes GL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//includes du projet
#include "utils.h"
//Includes ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//variables globales :

//variables pour utilisation du temps
float startTime = -1.0f;
float cooldown = 0.25f; // Durée du cooldown en secondes
float t_press = 0.0f; // Temps écoulé depuis la dernière pression