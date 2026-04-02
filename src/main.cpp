//OpenGL et rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <ostream>
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
//autre
#include <string>
#include <string.h>
#include <random>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iterator>
//headers
#include "utils.h"
#include "mesh.h"
#include "function.h"
#include "camera.h"

//GESTION FENETRE
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//Affichage ou non de la fenêtre IMGUI (activer/désactiver avec 'H')
bool show_ui = true;


//GESTION INPUTS
//gestion input clavier : ici, si KEY_ESCAPE préssée
void processInput(GLFWwindow *window, bool* moveRight, bool* moveLeft, bool* moveUp, bool* moveDown, bool* pressR, bool* pressA, bool* pressO, bool* pressH){
     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    *moveRight = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    *moveLeft = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    *moveUp = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    *moveDown = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
    *pressR = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
    *pressA = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    *pressO = (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS);
    *pressH = (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS);
}

int main(int argc, char* argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    
    //Création objet fenêtre
    GLFWwindow* window = glfwCreateWindow(800, 600, "Display Function", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Initialisation de GLAD 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Permettre de régler gl_PointSize depuis le vertex shader
    glEnable(GL_PROGRAM_POINT_SIZE);
    //Pour la profondeur
    glEnable(GL_DEPTH_TEST);
    
    //position et dimensions de la fenêtre
    glViewport(0, 0, 800, 600);

    //possibililté de resize 
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    //préciser que l'ont veut qu'il resize régulièrement
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Caméra : callback pour la position de la souris
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    
    //Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    
//DEF DES SHADERS
    //Def de Shader (basique) à travers un C String
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    //"layout (location = 1) in vec3 aNormal;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n" 
    "uniform mat4 projection;\n"
    "out vec3 Normal;\n"
    "out vec3 FragPos;\n"    
    "void main()\n"
    "{\n"
    "FragPos = vec3(model * vec4(aPos, 1.0));\n"
    //"Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "Normal = vec3(0.0, 0.0, 1.0);\n"
    "gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\0";


    //création objet Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // -> type de shader

    

    //Association de l'objet et de notre shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    //compilation
    glCompileShader(vertexShader);

    //Idem que le vertex shader mais avec la couleur
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    //main
    "void main() {\n"
    // Lumière basique
    "vec3 lightPos = vec3(1.0, 1.0, 2.0);\n"
    "vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
    "vec3 objectColor = vec3(0.5, 0.2, 0.2);\n" // Rouge brique Blender
    // Ambiant
    "float ambientStrength = 0.3;\n"
    "vec3 ambient = ambientStrength * lightColor;\n"
    "vec3 norm = normalize(Normal);\n"
    "vec3 lightDir = normalize(lightPos - FragPos);\n"
    "float diff = max(dot(norm, lightDir), 0.0);\n"
    "vec3 diffuse = diff * lightColor;\n"
    "vec3 result = (ambient + diffuse) * objectColor;\n"
    //essai profondeur
    "float depth = (FragPos.z + 0.3);\n"
    "FragColor = vec4(vec3(depth), 1.0);\n"  //ombre
    //"FragColor = vec4(result, 1.0);\n"  //couleur
    "}\0";   

//HEY REPRENDS LA FINIS DE SET LES FRAGMENTS SHADERS !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//création objet Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // -> type de shader
    //Association de l'objet et de notre shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //compilation
    glCompileShader(fragmentShader);

    //Creer objet programme
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //attache les objets au programme
    //essayons d'en garder qu'un seul
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //appel au programme 
    glUseProgram(shaderProgram);

    //On supprime les objets après les avoir attaché
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //On précise à OpenGL comment interpréter nos données pour les afficher
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),  (void*)0);
    glEnableVertexAttribArray(0);

    GLuint VBO, EBO, VAO;
    //Attribution des vertices aux objets (on tient compte de quels vertices forment des arrêtes avec quelles vertices)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    calculate(100, 100);
    //.data() pour avoir les données comme float*
    glBufferData(GL_ARRAY_BUFFER, surf.vertices.size() * sizeof(float), surf.vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, surf.triangles.size()* sizeof(unsigned int), surf.triangles.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    
    //pour le dessin
    int indexCount = surf.vertices.size() / 3;
    int n = indexCount;
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    float minx = 1e9f, miny = 1e9f, minz = 1e9f;
    float maxx = -1e9f, maxy = -1e9f, maxz = -1e9f;
    for (int i = 0; i < n; ++i) {
        float x = surf.vertices[3*i + 0];
        float y = surf.vertices[3*i + 1];
        float z = surf.vertices[3*i + 2];
        cx += x; cy += y; cz += z;
        minx = std::min(minx, x); miny = std::min(miny, y); minz = std::min(minz, z);
        maxx = std::max(maxx, x); maxy = std::max(maxy, y); maxz = std::max(maxz, z);
    }
    cx /= n; cy /= n; cz /= n;
    float rangeX = maxx - minx;
    float rangeY = maxy - miny;
    float rangeZ = maxz - minz;
    float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));
    float fitScale = (maxRange > 0.0f) ? (2.0f / maxRange) : 1.0f; // mappe la plus grande étendue sur [-1,1]*/

    camera.reset();

//render loop (maintient la fenêtre ouverte, une loop = une frame)
    //se divise en 4 parties : nettoyage, input, render puis cloture
    while(!glfwWindowShouldClose(window)){
        //mesure du temps pour animation
        float currentTime = glfwGetTime();
        
        //camera :
        // 1. Calcul de la position de la caméra sur la sphère
        glm::vec3 pos;
        pos.x = camera.cameraTarget.x + camera.cameraDistance * cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw));
        pos.y = camera.cameraTarget.y + camera.cameraDistance * sin(glm::radians(camera.pitch));
        pos.z = camera.cameraTarget.z + camera.cameraDistance * cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw));

        // 2. Création de la matrice View
        glm::mat4 view = glm::lookAt(pos, camera.cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)); //On modifie là où regarde la caméra


//P1 : nettoyage
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Aussi GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
        
        
//P2 : gestion input clavier
        bool moveRight = false;
        bool moveLeft = false;
        bool moveUp = false;
        bool moveDown = false;
        bool pressR = false;
        //Set Phonemes pour tester l'anim ;)
        bool pressA = false;
        bool pressO = false;
        bool pressH = false;
        processInput(window, &moveRight, &moveLeft, &moveUp, &moveDown, &pressR, &pressA, &pressO, &pressH);


//P3 : gestion du render

        //Attention : au choix du programme Shader utilisé

        
        //réaction inputs
        if(moveUp){
            camera.viewz += 0.05;
        }
        if(moveDown){   
            camera.viewz -= 0.05; 
        }
        if(moveRight){   
            camera.viewx += 0.05; 
        }
        if(moveLeft){   
            camera.viewx -= 0.05; 
        }
        if(pressR){   
            camera.reset(); 
            x_min = -15.0f; x_max = 15.0f; y_min = -15.0f; y_max = 15.0f; z_min = 15.0f; z_max = -15.0f;
            calculate(100, 100);
            //autres trucs à reset
        }

        //dessin du triangle
        glUseProgram(shaderProgram);

        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //centrer, tourner, scale (ordre)
        model = glm::scale(model, glm::vec3(fitScale*0.5f)); // Utilise fitScale calculé 
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //axe de profil
        //model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //flip le masque
        model = glm::translate(model, glm::vec3(-cx, -cy, -cz)); // Centre le masque
        //view = glm::lookAt(glm::vec3(camera.viewx, camera.viewy, camera.viewz), glm::vec3(0, -0.18, 0), glm::vec3(0, 1, 0)); //position de la cam, vers où elle regarde, up vecteur
        //glm::mat4 view = glm::lookAt(glm::vec3(2.0f, -2.0f, 2.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //position de la cam, vers où elle regarde, up vecteur
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        /*
        //glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //rotation car le masque est vers le haut dans Blender
        //centrer, tourner, scale (ordre)
        model = glm::scale(model, glm::vec3(fitScale*0.5f)); // Utilise fitScale calculé 
        //le masque était à l'envers dans Blender, on l'a remis à l'endroit depuis
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //axe de profil
        //model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //flip la surface
        model = glm::translate(model, glm::vec3(-cx, -cy, -cz)); // Centre la surface
        //Utilisation plus tôt, on enlève la définition (glm::mat4 ...)
        //view = glm::lookAt(glm::vec3(camera.viewx, camera.viewy, camera.viewz), glm::vec3(0, -0.18, 0), glm::vec3(0, 1, 0)); //position de la cam, vers où elle regarde, up vecteur
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);*/


        // Envoie au shader
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //glUniform3f(loc_min, minx, miny, minz);
        //glUniform3f(loc_max, maxx, maxy, maxz);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //essaye GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
        glDrawElements(GL_TRIANGLES, surf.triangles.size(), GL_UNSIGNED_INT, 0);
        //calculate(100, 100);

        //glBufferData(GL_ARRAY_BUFFER, surf.size() * sizeof(float), surf.vertices.data(), GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, surf.vertices.size() * sizeof(float), surf.vertices.data());

        if(edited){
            calculate(100, 100);
            glBufferSubData(GL_ARRAY_BUFFER, 0, surf.vertices.size() * sizeof(float), surf.vertices.data());
            edited = false;
        }
        //Gestion de pression des touches :
        if(startTime >= 0.0f){
            //Fragmentation du cooldown
            t_press = (currentTime - startTime)/cooldown;
            //Fin du cooldown
            if (t_press >= 1.0f){
                t_press = 1.0f;
                startTime = -1.0f;
            }
        }

        //pour cacher/afficher la fenêtre IMGUI 
        if (pressH && startTime < 0.0f) {    
            show_ui = !show_ui; // 'H' pour Hide
            startTime = currentTime; // Démarre le cooldown
        }
    
    // 3.5. IMGUI (Interface par dessus la 3D)
        if(show_ui){
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // -- Positionnement de la fenêtre (Point 1) --
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->Pos;
            ImVec2 work_size = viewport->Size;
            ImVec2 window_pos, window_pos_pivot;

            // On centre horizontalement (x = 0.5) et on ancre en bas (y = 1.0)
            window_pos.x = work_pos.x + work_size.x * 0.5f;
            window_pos.y = work_pos.y + work_size.y - 50.0f; // 50px de marge en bas
            window_pos_pivot.x = 0.5f;
            window_pos_pivot.y = 1.0f;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowSize(ImVec2(450, 250)); // Taille fixe pour être joli

            // -- Contenu de la fenêtre --
            ImGui::Begin("Input Panel", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); // NoDecoration retire la barre de titre bleue

            ImGui::Text("Enter a x-range starting value:");

            // Point 2 : Input Text
            // "##Input" cache le label à gauche, on utilise Text() au dessus à la place
            // ImGui_InputTextFlags_EnterReturnsTrue permet de valider avec la touche ENTRÉE aussi
            bool enterPressed = ImGui::InputText("##Input", xminIB, IM_ARRAYSIZE(xminIB), ImGuiInputTextFlags_EnterReturnsTrue);   
            ImGui::Text("Enter a x-range ending value:"); 
            bool enterPressed2 = ImGui::InputText("##Input2", xmaxIB, IM_ARRAYSIZE(xmaxIB), ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::Text("Enter a y-range starting value:");
            bool enterPressed3 = ImGui::InputText("##Input3", yminIB, IM_ARRAYSIZE(yminIB), ImGuiInputTextFlags_EnterReturnsTrue);  
            ImGui::Text("Enter a y-range ending value:");  
            bool enterPressed4 = ImGui::InputText("##Input4", ymaxIB, IM_ARRAYSIZE(ymaxIB), ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::Text("Set a maximum height to truncate the display:");  
            bool enterPressed5 = ImGui::SliderFloat("##Input5", &z_min, 1.0f, 100.0f, "%.1f");
            ImGui::Text("Set a minimum height to truncate the display:");  
            bool enterPressed6 = ImGui::SliderFloat("##Input6", &z_max, -100.0f, -1.0f, "%.1f");

            // Point 3 : Bouton qui remplace le terminal
            //ImGui::SameLine(); // Met le bouton à droite du champ texte
            if (ImGui::Button("Edit") || enterPressed || enterPressed2 || enterPressed3 || enterPressed4 || enterPressed5 || enterPressed6){
                //position boutton
                // Convertit le char* en float c++
                std::string sentence0(xminIB);
                x_min = (float)atof(sentence0.c_str());          
                std::string sentence1(xmaxIB);
                x_max = (float)atof(sentence1.c_str());
                std::string sentence2(yminIB);
                y_min = (float)atof(sentence2.c_str());
                std::string sentence3(ymaxIB);
                y_max = (float)atof(sentence3.c_str());
                // Appelle de la fonction de traitement 
                printf("(x, y) range set to : [%f, %f] x [%f, %f]\n", minx, maxx, miny, maxy); 
                edited = true;

                // Remettre le focus sur l'input après clic (optionnel)
                ImGui::SetKeyboardFocusHere(-1); 
            }

            ImGui::End();

            // Rendu ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    //P4 : fin render loop
        //met les pixels en couleur
        glfwSwapBuffers(window);
        //vérifie si un input a été trigger
        glfwPollEvents();
        //Nettoyage ImGui
    }
    
    //Nettoyage ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    printf("Function display window closed\n");
    glfwTerminate();
    return 0;
}
