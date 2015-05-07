//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_POINT_ATOM_H
#define PROJECT_POINT_ATOM_H

#include "main.h"
#include "point.h"

class PointAtom {
private:
    Property _property;
    Point _point;
public:
    PointAtom(Property const &_property, const Point &_point) : _property(_property), _point(_point) { }


    Property const &get_property() const;

    Point const &get_point() const;

    bool operator==(const PointAtom &other) const;

    bool operator<(const PointAtom &other) const;

    void print() const;
};


#endif //PROJECT_POINT_ATOM_H
