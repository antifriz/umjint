//
// Created by ivan on 5/7/15.
//

#include "point.h"

void Point::print() const {
    std::cout << "(" << x << "," << y << ")";
}

bool Point::operator==(const Point &b) const {

    return x == b.x && y == b.y;
}

bool Point::operator!=(const Point &other) const {
    return !(*this == other);
}

bool Point::operator<(const Point &other) const {
    if (x < other.x) return true;
    if (other.x < x) return false;
    return y < other.y;
}
