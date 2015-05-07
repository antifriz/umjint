//
// Created by ivan on 5/7/15.
//

#include "point_atom.h"

Property const &PointAtom::get_property() const {
    return _property;
}

Point const &PointAtom::get_point() const {
    return _point;
}

bool PointAtom::operator==(const PointAtom &other) const {
    return _point == other._point && _property == other._property;
}

bool PointAtom::operator<(const PointAtom &other) const {
    return _point == other._point ? _property < other._property : _point < other._point;
}

void PointAtom::print() const {
    char c = '?';
    if (_property == Wumpus) c = 'W'; else if (_property == Pit) c = 'P'; else if (_property == Glow)
        c = 'G';
    else if (_property == Teleport) c = 'T'; else if (_property == Breeze) c = 'B'; else if (_property == Stench)
        c = 'S';
    std::cout << c;
    _point.print();
}