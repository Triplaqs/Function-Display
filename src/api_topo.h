#ifndef API_TOPO_H
#define API_TOPO_H

//outils pour la géométrie c++
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//headers
#include <iostream>
#include <string>
//#include <cpr/cpr.h> // get API
#include <curl/curl.h> // get API v2 essai
//#include <nlohmann/json.hpp> // use API
#include "../includes/json.hpp" // use API vé idem
#include <regex>

#include "utils.h"
#include "mesh.h"
#include "api_topo.h"



size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

//lit le retour de l'API pour en extraire l'élévation
float extractElevation(const std::string& jsonResponse);

//va chercher l'élévation à une latitude et longitude données en appelant l'API
float callElevationAPI(float lat, float lon);

#endif