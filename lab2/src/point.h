//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_POINT_H
#define PROJECT_POINT_H

class Point {
public:

    const int x;
    const int y;

    Point(const int &x, const int &y) : x(x), y(y) { }

    Point operator=(const Point &p) {
        return Point(p);
    }


    bool operator<(const Point &other) const {
        if (x < other.x) return true;
        if (other.x < x) return false;
        return y < other.y;
    }

    bool operator!=(const Point &other) const {
        return !(*this == other);
    }

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }


    void print() const {
        std::cout << "(" << x << "," << y << ")";
    }
};

#include <string>
#include <vector>
#include <iomanip>
#include <queue>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <set>

#endif //PROJECT_POINT_H
