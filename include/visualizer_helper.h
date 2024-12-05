#pragma once
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <complex>
#include <vector>
#include <thread>
#include "convex_hull.h"
#define PLOT_VIS true

// URL del servidor Flask
const std::string FLASK_URL = "http://127.0.0.1:8000/update";

// Función para enviar actualizaciones al visualizador
void send_update(const std::vector<P>& points,
                 const std::vector<std::pair<P, P  >>& hull_edges);