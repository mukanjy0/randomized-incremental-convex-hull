//
// Created by iansg on 12/5/2024.
//

#ifndef CONVEX_HULL_PARALLELCH_H
#define CONVEX_HULL_PARALLELCH_H

#include <algorithm>
#include <complex>
#include <map>
#include <set>
#include <vector>
#include <stack>
using namespace std;

#define inf LLONG_MAX
typedef long long D;
typedef pair<D,D> P;
#define X first
#define Y second
#define facet pair<P,P>
#define ridge P
#define supportSet pair<facet,facet>

D operator*(P p1, P p2);
P operator-(P p1, P p2);
ostream& operator<<(ostream& os, P p);

class ParallelCH {
    // number of points
    size_t N;
    // X = set of points
    vector<P> V;
    // maps facet to conflicting points
    map<facet,vector<size_t>> C;
    // convex hull (incrementally updated)
    set<facet> H;
    // maps ridge to the first found facet that contains it
    map<ridge,facet> M;

    static bool visible(P& p, facet& t);

    static bool leftTurn(P& p, facet& t);
    static bool rightTurn(P& p, facet& t);

    size_t minC(facet t);

    // recall from definition of support set that (C[t] U {x}) in C[t1] U C[t2] : {t1,t2} is a support set
    void setCFromSS(facet t, supportSet SS);

    void burySS(supportSet SS);

    void processRidge(facet t1, ridge r, facet t2);

public:
    ParallelCH(size_t N, vector<P> points) : N(N), V(points) {}

    set<facet> parallelCH(int d=2);

    static vector<facet> CHS(vector<P> p);
    static vector<facet> CH(vector<P> points);

    static pair<vector<P>, vector<facet>> getCHGraph(vector<facet>& hull);

    static pair<vector<P>, set<facet>> getCHGraph(set<facet>& hull);
};
#endif //CONVEX_HULL_PARALLELCH_H
