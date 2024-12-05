#ifndef SEQUENTIAL_CONVEX_HULL_H
#define SEQUENTIAL_CONVEX_HULL_H

#include <iostream>
#include <utility>
#include <set>
#include <complex>
#include <map>


typedef long long D;
typedef std::pair<D,D> P;
#define X first
#define Y second
#define facet std::pair<P,P>
#define ridge P

D operator*(P p1, P p2);
P operator-(P p1, P p2);


facet swapComponents(facet f);

bool isVisible(P v, facet  t);

std::vector<P> ConvexHull(std::vector<P>& points);






#endif  //SEQUENTIAL_CONVEX_HULL_H