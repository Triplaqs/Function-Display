//#include <cpr/cpr.h> // get API
#include <curl/curl.h> // get API v2 essai
//#include <nlohmann/json.hpp> // use API
#include "../includes/json.hpp" // use API vé idem
#include <regex>
#include <iostream>
#include <string>

#include "utils.h"
#include "mesh.h"
#include "api_topo.h"


// Fonction helper pour récupérer la réponse
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

float extractElevation(const std::string& jsonResponse) {
    std::string key = "\"elevation\":";
    size_t pos = jsonResponse.find(key);
    
    if (pos == std::string::npos) return 0.0f; // Non trouvé

    // On se place après "elevation": 
    pos += key.length();

    // On cherche le début du nombre (on saute les espaces)
    size_t start = jsonResponse.find_first_of("0123456789-", pos);
    // On cherche la fin du nombre (la virgule qui suit)
    size_t end = jsonResponse.find_first_of(",", start);

    if (start != std::string::npos && end != std::string::npos) {
        std::string valueStr = jsonResponse.substr(start, end - start);
        return std::stof(valueStr); // Convertit "ab.c" en float
    }
    
    printf("Error: could not connect to Open Topo Data. Check your Internet connexion.\n");
    return 0.0f;
}

float callElevationAPI(float lat, float lon){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        printf("Calling Open Topo Data API for lat: %f, lon: %f\n", lat, lon);
        std::string url = "https://api.opentopodata.org/v1/srtm90m?locations=" 
                          + std::to_string(lat) + "," + std::to_string(lon);
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        return extractElevation(readBuffer); // Contient le JSON en texte brut
    }

    printf("Error: could not connect to Open Topo Data. Check your Internet connexion.\n");
    return 0.0f;
}

