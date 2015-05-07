//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_POINT_ATOM_H
#define PROJECT_POINT_ATOM_H

class PointAtom {
private:
    Property _property;
    Point _point;
public:
    PointAtom(Property const &_property, const Point &_point) : _property(_property), _point(_point) { }


    Property const &get_property() const {
        return _property;
    }

    Point const &get_point() const {
        return _point;
    }

    bool operator==(const PointAtom &other) const {
        return _point == other._point && _property == other._property;
    }

    bool operator<(const PointAtom &other) const {
        return _point == other._point ? _property < other._property : _point < other._point;
    }

    void print() const {
        char c = '?';
        if (_property == Wumpus) c = 'W'; else if (_property == Pit) c = 'P'; else if (_property == Glow)
            c = 'G';
        else if (_property == Teleport) c = 'T'; else if (_property == Breeze) c = 'B'; else if (_property == Stench)
            c = 'S';
        std::cout << c;
        _point.print();
    }
};

#include <string>
#include <vector>
#include <iomanip>
#include <queue>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <set>
#include <iostream>
#include "point.h"

#endif //PROJECT_POINT_ATOM_H
