//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H

#include "lazy_defines.h"
#include <vector>

#include "field_data.h"
#include "point.h"
#include <set>

class Board {
private:
    std::vector<FieldData> _fields;
    int width;
    int height;
public:
    Board() { }

    void print() const;

    void print(std::set<Point> visited, Point current) const;

    void load(std::string path);

    const FieldData & getData(const Point &point) const;


    template<Property property>
    bool is(const Point &point) const {
        return getData(point).is<property>();
    }

    std::vector<Point> getNeighbours(const Point &position) const;


};


#endif //PROJECT_BOARD_H
