//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_ATOM_H
#define PROJECT_ATOM_H

#include "point.h"

enum Property { Glow, Breeze, Stench, Teleport, Pit, Wumpus };

class Atom {
private:
    Property _property;
    Point _point;
public:
    Atom() : _point(1, 1) { }

    Atom(Property const &_property, const Point &_point) : _property(_property), _point(_point) { }

    bool operator==(const Atom &other) const;

    bool operator<(const Atom &other) const;

    void print() const;
};


#endif //PROJECT_ATOM_H
