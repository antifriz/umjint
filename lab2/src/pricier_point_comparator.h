//
// Created by ivan on 5/15/15.
//

#ifndef PROJECT_PRICIER_POINT_COMPARATOR_H
#define PROJECT_PRICIER_POINT_COMPARATOR_H

#include "point.h"

class ComparatorPricierPoint {
public:
    bool operator()(const Point &a, const Point &b) {
        return (a.x * 10 + a.y) == (b.x * 10 + b.y) ? (a < b) : (a.x * 10 + a.y) > (b.x * 10 + b.y);
    }
};


#endif //PROJECT_PRICIER_POINT_COMPARATOR_H
