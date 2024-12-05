//
// Created by iansg on 12/5/2024.
//

#include "ParallelCH.h"

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

ostream& operator<<(ostream& os, P p) {
    os << p.X << ' ' << p.Y << '\n';
    return os;
}

bool ParallelCH::visible(P& p, facet& t) {
    auto [s1, s2] = t;

    // cross-product > 0 indicates turn to the left
    // since CH is constructed clockwise, then turn left means that the point is outside the hull
    return ((s2 - s1) * (p - s1)) > 0;
}

bool ParallelCH::leftTurn(P& p, facet& t) {
    auto [s1, s2] = t;
    return ((s2 - s1) * (p - s1)) > 0;
}
bool ParallelCH::rightTurn(P& p, facet& t) {
    auto [s1, s2] = t;
    return ((s2 - s1) * (p - s1)) < 0;
}

size_t ParallelCH::minC(facet t) {
    return C[t].empty() ? inf : *begin(C[t]);
}

// recall from definition of support set that (C[t] U {x}) in C[t1] U C[t2] : {t1,t2} is a support set
void ParallelCH::setCFromSS(facet t, supportSet SS) {
    auto [t1, t2] = SS;

    // update C[t] from C[t1]
    for (auto i : C[t1]) {
        P point = V[i];
        if (visible(point, t)) {
            C[t].push_back(i);
        }
    }

    // update C[t] from C[t2]
    for (auto i : C[t2]) {
        P point = V[i];
        if (visible(point, t)) {
            C[t].push_back(i);
        }
    }
}

void ParallelCH::burySS(supportSet SS) {
    auto [t1, t2] = SS;

    H.erase(H.find(t1));
    C.erase(t1);

    H.erase(H.find(t2));
    C.erase(t2);
}

void ParallelCH::processRidge(facet t1, ridge r, facet t2) {
    // ensure that {t1, t2} is a valid support set
    if (C[t1].empty() && C[t1].empty()) return;

    // p is first evaluated point for which {t1, t2} is a support set

    // if p is visible from both, then the dependence is *buried*
    if (minC(t1) == minC(t2)) {
        burySS({t1, t2});
        return;
    }

    // ensure that p is visible from t1, not t2
    if (minC(t1) > minC(t2)) return processRidge(t2, r, t1);

    // process conflict (true dependence)
    P p = V[minC(t1)];
    facet t = {r, p};
    setCFromSS(t, {t1, t2});

    H.erase(H.find(t1));
    H.insert(t);
    C.erase(t1);

    processRidge(t, r, t2);

    // for 2D, ridges of t are only {r, p}
    ridge r_prime = p;
    if (M.count(r)) {
        auto t_prime = M[r];
        processRidge(t_prime, r_prime, t);
    }
    else {
        M[r] = t;
    }
}

set<facet> ParallelCH::parallelCH(int d) {
    int nb = d+1;
    vector<facet> hull = CHS(vector<P>(begin(V), begin(V) + nb));
    H = set<facet>(begin(hull), end(hull));

    for (facet& t : hull) {
        for (size_t i = 0; i < N; ++i) {
            if (visible(V[i], t)) {
                C[t].push_back(i);
            }
        }
    }

    auto n = hull.size();
    // note that, since H is ordered, then each pair of consecutive edges (cyclic) shares a ridge
    for (int i = 0; i < n; ++i) {
        facet t1 = hull[i];
        facet t2 = hull[(i + 1) % n];
        // second point of t1 equals first point of t2
        // this shared point is the intersection between the facets, hence a ridge
        ridge r = t1.Y;
        processRidge(t1, r, t2);
    }

    return H;
}

vector<facet> ParallelCH::CHS(vector<P> p) {
    sort(begin(p), end(p));
    return {{p[0], p[1]}, {p[1], p[2]}, {p[2], p[0]}};
}

vector<facet> ParallelCH::CH(vector<P> points) {
    sort(begin(points), end(points));

    stack<P> bottom;
    bottom.push(points[0]);
    bottom.push(points[1]);
    facet lastEdge = {points[0], points[1]};
    size_t n = points.size();
    for (size_t i = 2; i < n; ++i) {
        P point = points[i];

        while (rightTurn(point, lastEdge)) {
            auto [u, v] = lastEdge;
            bottom.pop();
            lastEdge.second = u;

            if (bottom.size() == 1) break;
            bottom.pop();

            lastEdge = {bottom.top(), u};
            bottom.push(u);
        }
        bottom.push(point);
        lastEdge.first = lastEdge.second;
        lastEdge.second = point;
    }

    stack<P> up;
    up.push(points[0]);
    up.push(points[1]);
    lastEdge = {points[0], points[1]};
    for (size_t i = 2; i < n; ++i) {
        P point = points[i];

        while (leftTurn(point, lastEdge)) {
            auto [u, v] = lastEdge;
            up.pop();
            lastEdge.second = u;

            if (up.size() == 1) break;
            up.pop();

            lastEdge = {up.top(), u};
            up.push(u);
        }
        up.push(point);
        lastEdge.first = lastEdge.second;
        lastEdge.second = point;
    }

    vector<facet> hull;
    P prev = bottom.top();
    bottom.pop();
    while (!bottom.empty()) {
        P p = bottom.top();
        bottom.pop();
        hull.emplace_back(prev, p);
        prev = p;
    }

    while (!up.empty()) {
        bottom.push(up.top());
        up.pop();
    }

    prev = bottom.top();
    bottom.pop();
    while (!bottom.empty()) {
        P p = bottom.top();
        bottom.pop();
        hull.emplace_back(prev, p);
        prev = p;
    }

    return hull;
}

pair<vector<P>, vector<facet>> ParallelCH::getCHGraph(vector<facet>& hull) {
    set<P> points;
    for (facet t : hull) {
        auto [s1, s2] = t;
        points.insert(s1);
        points.insert(s2);
    }

    return {vector<P>(begin(points), end(points)), hull};
}

pair<vector<P>, set<facet>> ParallelCH::getCHGraph(set<facet>& hull) {
    set<P> points;
    for (facet t : hull) {
        auto [s1, s2] = t;
        points.insert(s1);
        points.insert(s2);
    }

    return {vector<P>(begin(points), end(points)), hull};
}
