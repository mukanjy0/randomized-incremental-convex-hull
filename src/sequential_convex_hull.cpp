#include "visualizer_helper.h"
#include <set>
#include <map>
#include <vector>
#include <complex>
#include <random>
#include <algorithm>
#include <iostream>
#include "sequential_convex_hull.h"


std::random_device rd;
std::mt19937 g(rd());

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

std::vector<P> ConvexHull(std::vector<P>& points) {
    size_t n = points.size();
    std::vector<P> hull;
    std::ranges::shuffle(points, g);

    if (n <= 3) {
        hull = points;
        return hull;
    }

    std::set<facet> H;
    std::map<facet, std::set<P>> C;
    std::map<P, std::set<facet>> C_inv;

    for (int i = 0; i < 3; i++) {
        auto current_point = points[(i + 2) % 3];
        facet current_facet = {points[i % 3], points[(i + 1) % 3]};
        if (isVisible(current_point, current_facet)) {
            H.insert({points[(i + 1) % 3], points[i % 3]});
        } else {
            H.insert({points[i % 3], points[(i + 1) % 3]});
        }
    }

    for (auto& f : H) {
        C[f] = {};
        for (const auto& p : points) {
            if (isVisible(p, f)) {
                C[f].insert(p);
                C_inv[p].insert(f);
            }
        }
    }

    std::vector<P> vis_points(points.begin(), points.end());
    std::vector<std::pair<P,P>> vis_edges;
    vis_edges.reserve(H.size());
    for (const auto& f : H) {
        vis_edges.emplace_back(f.first, f.second);
    }
    send_update(vis_points, vis_edges);

    for (size_t i = 3; i < n; i++) {
        P vi = points[i];
        auto R = C_inv[vi];

        std::set<ridge> boundary;
        for (const auto& t : R) {
            ridge r1 = t.first;
            ridge r2 = t.second;
            if (boundary.find(r1) != boundary.end()) boundary.erase(r1);
            else boundary.insert(r1);
            if (boundary.find(r2) != boundary.end()) boundary.erase(r2);
            else boundary.insert(r2);
        }

        for (const auto& r : boundary) {
            facet t1, t2;
            for (const auto& f : R) {
                if (f.first == r || f.second == r) {
                    if (isVisible(vi, f)) {
                        t1 = f;
                    } else {
                        t2 = f;
                    }
                }
            }

            facet new_f = {r, vi};
            if (isVisible(t1.first, new_f)) new_f = swapComponents(new_f);

            H.insert(new_f);
            C[new_f] = {};

            for (const auto& vis_p : C[t1]) {
                if (isVisible(vis_p, new_f)) {
                    C[new_f].insert(vis_p);
                    C_inv[vis_p].insert(new_f);
                }
            }
            for (const auto& vis_p : C[t2]) {
                if (C[new_f].find(vis_p) == C[new_f].end() && isVisible(vis_p, new_f)) {
                    C[new_f].insert(vis_p);
                    C_inv[vis_p].insert(new_f);
                }
            }
        }

        for (const auto& t : R) {
            H.erase(t);
            for (const auto& vis_p : C[t]) {
                C_inv[vis_p].erase(t);
            }
            C.erase(t);
        }

        vis_edges.clear();
        for (const auto& f : H) {
            vis_edges.emplace_back(f.first, f.second);
        }
        send_update(vis_points, vis_edges);
    }

    for (const auto& f : H) {
        if (hull.empty() || hull.back() != f.first) {
            hull.push_back(f.first);
        }
        hull.push_back(f.second);
    }

    return hull;
}