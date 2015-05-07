//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_POINT_H
#define PROJECT_POINT_H

#include <iostream>

class Point {
public:

    const int x;
    const int y;

    Point(const int &x, const int &y) : x(x), y(y) { }

    Point operator=(const Point &p);

    bool operator<(const Point &other) const;

    bool operator!=(const Point &other) const;

    bool operator==(const Point &other) const;

    void print() const;
};


#endif //PROJECT_POINT_H
