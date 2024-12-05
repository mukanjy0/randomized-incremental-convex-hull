#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include <chrono>
#include "convex_hull.h"

int main(int argc, char** argv) {
    std::random_device rd2;
    std::mt19937 gen2(rd2());

    // Modo de depuración opcional
    bool debug = false;
    if (argc > 1 && std::string(argv[1]) == "-debug") {
        debug = true;
    }

    // Generador de números aleatorios
    std::uniform_real_distribution<> dis(0.0, 100.0);

    // Iterar desde 10 puntos hasta 10,000 puntos
    for (int num_points = 10; num_points <= 1000000; num_points *= 10) {
        // Generar puntos aleatorios
        std::vector<P> points;
        for (int i = 0; i < num_points; i++) {
            points.emplace_back(dis(gen2), dis(gen2)); // Cada punto tiene coordenadas (x, y)
        }

        // Imprimir los puntos generados si está en modo debug
        if (debug) {
            std::cout << "Generated Points (" << num_points << " points):\n";
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
        std::cout << "Execution on " << num_points << " points took " << duration.count() << " microseconds.\n";

        // Imprimir el casco convexo calculado
        if (debug) {
            std::cout << "Convex Hull Points:\n";
            for (const auto& p : hull) {
                std::cout << "(" << p.X << ", " << p.Y << ")\n";
            }
        }
    }

    return 0;
}