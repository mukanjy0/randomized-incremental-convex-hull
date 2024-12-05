#include <vector>
#include "visualizer_helper.h"

int main() {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 0}, {0.5, 2}};
  std::vector<std::pair<Point, Point>> hull_edges = {
          {{0, 0}, {1, 1}}, {{1, 1}, {2, 0}}, {{2, 0}, {0, 0}}
  };

  send_update(points, hull_edges);

  return 0;
}