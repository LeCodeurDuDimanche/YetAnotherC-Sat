// This file was taken from https://mpen.ca/406/bayazit

// Written by Mark Bayazit (darkzerox)
// March 23, 2009

#include <vector>
#include <algorithm>
#include <limits>
#include "hitbox.h"

typedef double Scalar;

int wrap(const int &a, const int &b);
template<class T> T& at(std::vector<T> v, int i) {
    return v[wrap(i, v.size())];
};

std::vector<Point> steinerPoints, reflexVertices;
Point operator+(const Point &a, const Point &b) {
    return Point(a.x + b.x, b.y + b.y);
}

Scalar area(const Point &a, const Point &b, const Point &c) {
    return (((b.x - a.x)*(c.y - a.y))-((c.x - a.x)*(b.y - a.y)));
}

bool left(const Point &a, const Point &b, const Point &c) {
    return area(a, b, c) > 0;
}

bool leftOn(const Point &a, const Point &b, const Point &c) {
    return area(a, b, c) >= 0;
}

bool right(const Point &a, const Point &b, const Point &c) {
    return area(a, b, c) < 0;
}

bool rightOn(const Point &a, const Point &b, const Point &c) {
    return area(a, b, c) <= 0;
}

bool collinear(const Point &a, const Point &b, const Point &c) {
    return area(a, b, c) == 0;
}

Scalar sqdist(const Point &a, const Point &b) {
    Scalar dx = b.x - a.x;
    Scalar dy = b.y - a.y;
    return dx * dx + dy * dy;
}

Scalar min(const Scalar &a, const Scalar &b) {
    return a < b ? a : b;
}

bool eq(const Scalar &a, const Scalar &b) {
    return abs(a - b) <= 1e-8;
}

int wrap(const int &a, const int &b) {
    return a < 0 ? a % b + b : a % b;
}

Scalar srand(const Scalar &min = 0, const Scalar &max = 1) {
    return rand() / (Scalar) RAND_MAX * (max - min) + min;
}

void makeCCW(std::vector<Point> &poly) {
    int br = 0;

    // find bottom right point
    for (unsigned i = 1; i < poly.size(); ++i) {
        if (poly[i].y < poly[br].y || (poly[i].y == poly[br].y && poly[i].x > poly[br].x)) {
            br = i;
        }
    }

    // reverse poly if clockwise
    if (!left(at(poly, br - 1), at(poly, br), at(poly, br + 1))) {
        reverse(poly.begin(), poly.end());
    }
}

bool isReflex(const std::vector<Point> &poly, const int &i) {
    return right(at(poly, i - 1), at(poly, i), at(poly, i + 1));
}

Point intersection(const Point &p1, const Point &p2, const Point &q1, const Point &q2) {
    Point i;
    Scalar a1, b1, c1, a2, b2, c2, det;
    a1 = p2.y - p1.y;
    b1 = p1.x - p2.x;
    c1 = a1 * p1.x + b1 * p1.y;
    a2 = q2.y - q1.y;
    b2 = q1.x - q2.x;
    c2 = a2 * q1.x + b2 * q1.y;
    det = a1 * b2 - a2*b1;
    if (!eq(det, 0)) { // lines are not parallel
        i.x = (b2 * c1 - b1 * c2) / det;
        i.y = (a1 * c2 - a2 * c1) / det;
    }
    return i;
}

void swap(int &a, int &b) {
    int c;
    c = a;
    a = b;
    b = c;
}

void decomposePoly(std::vector<Point>& poly, std::vector<std::vector<Point> >& decomposition) {
    Point upperInt, lowerInt, p, closestVert;
    Scalar upperDist, lowerDist, d, closestDist;
    unsigned upperIndex = 0, lowerIndex = 0, closestIndex = 0;
    std::vector<Point> lowerPoly, upperPoly;

    makeCCW(poly);

    for (unsigned i = 0; i < poly.size(); ++i) {
        if (isReflex(poly, i)) {
            reflexVertices.push_back(poly[i]);
            upperDist = lowerDist = std::numeric_limits<Scalar>::max();
            for (unsigned j = 0; j < poly.size(); ++j) {
                if (left(at(poly, i - 1), at(poly, i), at(poly, j))
                        && rightOn(at(poly, i - 1), at(poly, i), at(poly, j - 1))) { // if line intersects with an edge
                    p = intersection(at(poly, i - 1), at(poly, i), at(poly, j), at(poly, j - 1)); // find the point of intersection
                    if (right(at(poly, i + 1), at(poly, i), p)) { // make sure it's inside the poly
                        d = sqdist(poly[i], p);
                        if (d < lowerDist) { // keep only the closest intersection
                            lowerDist = d;
                            lowerInt = p;
                            lowerIndex = j;
                        }
                    }
                }
                if (left(at(poly, i + 1), at(poly, i), at(poly, j + 1))
                        && rightOn(at(poly, i + 1), at(poly, i), at(poly, j))) {
                    p = intersection(at(poly, i + 1), at(poly, i), at(poly, j), at(poly, j + 1));
                    if (left(at(poly, i - 1), at(poly, i), p)) {
                        d = sqdist(poly[i], p);
                        if (d < upperDist) {
                            upperDist = d;
                            upperInt = p;
                            upperIndex = j;
                        }
                    }
                }
            }

            // if there are no vertices to connect to, choose a point in the middle
            if (lowerIndex == (upperIndex + 1) % poly.size()) {
                p.x = (lowerInt.x + upperInt.x) / 2;
                p.y = (lowerInt.y + upperInt.y) / 2;
                steinerPoints.push_back(p);

                if (i < upperIndex) {
                    lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.begin() + upperIndex + 1);
                    lowerPoly.push_back(p);
                    upperPoly.push_back(p);
                    if (lowerIndex != 0) upperPoly.insert(upperPoly.end(), poly.begin() + lowerIndex, poly.end());
                    upperPoly.insert(upperPoly.end(), poly.begin(), poly.begin() + i + 1);
                } else {
                    if (i != 0) lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.end());
                    lowerPoly.insert(lowerPoly.end(), poly.begin(), poly.begin() + upperIndex + 1);
                    lowerPoly.push_back(p);
                    upperPoly.push_back(p);
                    upperPoly.insert(upperPoly.end(), poly.begin() + lowerIndex, poly.begin() + i + 1);
                }
            } else {
                // connect to the closest point within the triangle

                if (lowerIndex > upperIndex) {
                    upperIndex += poly.size();
                }
                closestDist = std::numeric_limits<Scalar>::max();
                for (unsigned j = lowerIndex; j <= upperIndex; ++j) {
                    if (leftOn(at(poly, i - 1), at(poly, i), at(poly, j))
                            && rightOn(at(poly, i + 1), at(poly, i), at(poly, j))) {
                        d = sqdist(at(poly, i), at(poly, j));
                        if (d < closestDist) {
                            closestDist = d;
                            closestVert = at(poly, j);
                            closestIndex = j % poly.size();
                        }
                    }
                }

                if (i < closestIndex) {
                    lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.begin() + closestIndex + 1);
                    if (closestIndex != 0) upperPoly.insert(upperPoly.end(), poly.begin() + closestIndex, poly.end());
                    upperPoly.insert(upperPoly.end(), poly.begin(), poly.begin() + i + 1);
                } else {
                    if (i != 0) lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.end());
                    lowerPoly.insert(lowerPoly.end(), poly.begin(), poly.begin() + closestIndex + 1);
                    upperPoly.insert(upperPoly.end(), poly.begin() + closestIndex, poly.begin() + i + 1);
                }
            }

            // solve smallest poly first
            if (lowerPoly.size() < upperPoly.size()) {
                decomposePoly(lowerPoly,decomposition);
                decomposePoly(upperPoly,decomposition);
            } else {
                decomposePoly(upperPoly,decomposition);
                decomposePoly(lowerPoly,decomposition);
            }
            return;
        }
    }
    decomposition.push_back(poly);
}
