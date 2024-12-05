#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include <chrono>
#include "parallel_convex_hull.h"

int main(int argc, char** argv) {
  // Generador de números aleatorios
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 100.0);

  // Iterar desde 10 puntos hasta 10,000 puntos
  for (int num_points = 10; num_points <= 1000000; num_points *= 10) {
    // Generar puntos aleatorios
    std::vector<P> points;
    for (int i = 0; i < num_points; i++) {
      points.emplace_back(dis(gen), dis(gen)); // Cada punto tiene coordenadas (x, y)
    }

    // Calcular el casco convexo
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<P> hull = ParallelConvexHull(points);
    auto stop = std::chrono::high_resolution_clock::now();

    // Calcular el tiempo de ejecución
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution on " << num_points << " points took " << duration.count() << " microseconds.\n";
  }
}
