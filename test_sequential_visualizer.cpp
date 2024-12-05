#include <iostream>
#include <random>

#include "sequential_convex_hull.h"

#define NUM_POINTS 10


std::random_device rdf;
std::mt19937 gf(rdf());

int main(int argc, char** argv)
{
  bool debug = false;
  if (argc > 1 && argv[1] == "-debug") {
    debug = true;
  }
  point2D *points = new point2D[NUM_POINTS];
  for (int i = 0; i < NUM_POINTS; i++) {
    //std::cin >> points[i].first >> points[i].second;
    // randomly generate points
    points[i] = std::make_pair(gf() % 100, gf() % 100);
  }
  //Sequential2DCH::printPoints2D(points, size);
  point2D *hull = new point2D[NUM_POINTS];
  auto start = std::chrono::high_resolution_clock::now();
  int hull_size = Sequential2DCH::convexHull2D(points, NUM_POINTS, hull, debug);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Execution on size " << NUM_POINTS << " dataset took " << duration.count() << " microseconds" << std::endl;
  //if (hull_size > 0) {
  //    cout << "Output size: " << hull_size << std::endl;
  //    printPoints2D(hull, hull_size);
  //}

  /*
  std::unordered_set<point2D> hull_set = std::unordered_set<point2D>();
  for (int i = 0; i < hull_size; i++) {
    hull_set.insert(hull[i]);
  }
  point2D *par_hull = new point2D[size];
  auto par_start = std::chrono:high_resolution_clock::now();
  int par_hull_size = Parallel2DCH:convexHull2D(points, size, par_hull, debug);
  auto par_stop = std::chrono:high_resolution_clock::now();
  auto par_duration = std::chrono::duration_cast<std::chrono::microseconds>(par_stop - par_start);
  std::cout << "Parallel execution on size " << size << " dataset took " << par_duration.count() << " microseconds" << std::endl;
  bool correct = true;
  if (par_hull_size != hull_size) {
    correct = false;
  } else {
    for (int i = 0; i < par_hull_size; i++) {
      if (hull_set.find(par_hull[i]) == hull_set.end()) {
        correct = false;
      }
    }
  }
  if (!correct) {
    std::cout << "Parallel solution INCORRECT" << std::endl;
  }
  */
  return 0;
}
