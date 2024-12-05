#include "visualizer_helper.h"
#include <set>
#include <map>
#include <vector>
#include <complex>
#include <random>
#include <algorithm>
#include <iostream>
#include "sequential_convex_hull.h"
#include "convex_hull.h"

D operator*(P p1, P p2) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return x1 * y2 - x2 * y1;
}

P operator-(P p1, P p2) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return {x1 - x2, y1 - y2};
}

facet swapComponents(facet f) {
    return {f.second, f.first};
}

bool isVisible(P v, facet t) {
    auto A = t.second - t.first;
    auto B = v - t.first;
    return A*B > 0;
}
