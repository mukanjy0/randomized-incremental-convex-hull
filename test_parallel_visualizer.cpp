#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "convex_hull.h"
#include "parallel_convex_hull.h"

int main(int argc, char** argv) {

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 100.0);

  for (int num_points = 10; num_points <= 10000; num_points *= 10) {
    std::vector<P> points;
    for (int i = 0; i < num_points; i++) {
      points.emplace_back(dis(gen), dis(gen)); // Cada punto tiene coordenadas (x, y)
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<P> hull = ParallelConvexHull(points);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution on " << num_points << " points took " << duration.count() << " microseconds.\n";
  }
}

