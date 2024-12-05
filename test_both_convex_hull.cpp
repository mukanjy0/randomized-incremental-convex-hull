#include <iostream>
#include <vector>
#include <complex>
#include <random>
#include <chrono>
#include "sequential_convex_hull.h"
#include "parallel_convex_hull.h"

int main() {
  // Random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 100.0);

  // Compare for increasing number of points
  for (int num_points = 10; num_points <= 1000000; num_points *= 10) {
    // Generate the same random points for both implementations
    std::vector<P> points;
    for (int i = 0; i < num_points; i++) {
      points.emplace_back(dis(gen), dis(gen));
    }

    // Sequential Convex Hull
    auto start_seq = std::chrono::high_resolution_clock::now();
    std::vector<P> seq_hull = ConvexHull(points);
    auto stop_seq = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::microseconds>(stop_seq - start_seq);

    // Parallel Convex Hull
    auto start_par = std::chrono::high_resolution_clock::now();
    std::vector<P> par_hull = ParallelConvexHull(points);
    auto stop_par = std::chrono::high_resolution_clock::now();
    auto duration_par = std::chrono::duration_cast<std::chrono::microseconds>(stop_par - start_par);

    // Results
    std::cout << "Number of Points: " << num_points << "\n";
    std::cout << "  Sequential Execution: " << duration_seq.count() << " microseconds\n";
    std::cout << "  Parallel Execution:   " << duration_par.count() << " microseconds\n";
    std::cout << "  Speedup:              " << (double)duration_seq.count() / duration_par.count() << "x\n\n";

    // Validate that both implementations produce the same result
    if (seq_hull != par_hull) {
      std::cerr << "Error: Sequential and Parallel implementations produced different results!\n";
    }
  }

  return 0;
}