#pragma once
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <complex>
#include <vector>
#include "sequential_convex_hull.h"
// URL del servidor Flask
const std::string FLASK_URL = "http://127.0.0.1:8000/update";

// Función para enviar actualizaciones al visualizador
void send_update(const std::vector<P>& points,
                 const std::vector<std::pair<P, P  >>& hull_edges) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        Json::Value root;
        Json::StreamWriterBuilder writer;

        // Convertir los puntos a JSON
        Json::Value points_json(Json::arrayValue);
        for (const auto& p : points) {
            Json::Value point_json;
            point_json.append(p.X); // Parte real (x)
            point_json.append(p.Y); // Parte imaginaria (y)
            points_json.append(point_json);
        }

        // Convertir las aristas del casco convexo a JSON
        Json::Value edges_json(Json::arrayValue);
        for (const auto& edge : hull_edges) {
            Json::Value edge_json(Json::arrayValue);
            Json::Value p1, p2;
            p1.append(edge.first.X); // Parte real de la primera coordenada
            p1.append(edge.first.Y); // Parte imaginaria de la primera coordenada
            p2.append(edge.second.X); // Parte real de la segunda coordenada
            p2.append(edge.second.Y); // Parte imaginaria de la segunda coordenada
            edge_json.append(p1);
            edge_json.append(p2);
            edges_json.append(edge_json);
        }

        root["points"] = points_json;
        root["hull_edges"] = edges_json;

        std::string json_data = Json::writeString(writer, root);

        // Configurar la solicitud POST de libcurl
        curl_easy_setopt(curl, CURLOPT_URL, FLASK_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_data.size());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Ejecutar la solicitud
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
}