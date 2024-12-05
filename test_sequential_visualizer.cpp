#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include <chrono>
#include "sequential_convex_hull.h"

// Número de puntos a generar
#define NUM_POINTS 10

int main(int argc, char** argv) {
    // Modo de depuración opcional
    bool debug = false;
    if (argc > 1 && std::string(argv[1]) == "-debug") {
        debug = true;
    }

    // Generador de números aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 100.0);

    // Generar puntos aleatorios
    std::vector<P> points;
    for (int i = 0; i < NUM_POINTS; i++) {
        points.emplace_back(dis(gen), dis(gen)); // Cada punto tiene coordenadas (x, y)
    }

    // Imprimir los puntos generados si está en modo debug
    if (debug) {
        std::cout << "Generated Points:\n";
        for (const auto& p : points) {
            std::cout << "(" << p.X << ", " << p.Y << ")\n";
        }
    }

    // Calcular el casco convexo
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<P> hull = ConvexHull(points);
    auto stop = std::chrono::high_resolution_clock::now();

    // Calcular el tiempo de ejecución
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution on " << NUM_POINTS << " points took " << duration.count() << " microseconds.\n";

    // Imprimir el casco convexo calculado
    if (debug) {
        std::cout << "Convex Hull Points:\n";
        for (const auto& p : hull) {
            std::cout << "(" << p.X << ", " << p.Y << ")\n";
        }
    }

    return 0;
}