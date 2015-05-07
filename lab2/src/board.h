//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H

#include "main.h"

class Board {
private:
    std::vector<FieldData> _fields;
    int width;
    int height;
public:
    Board() { }

    void print() const;

    std::vector<Point> getAllPoints();

    void load(std::string path);

    const FieldData & getData(const Point &point) const;


    template<Property property>
    bool is(const Point &point) const {
        return getData(point).is<property>();
    }

    std::vector<Point> getNeighbours(const Point &position) const {
        std::vector<Point> neigh;
        if (position.x > 2)
            neigh.emplace_back(position.x - 1, position.y);
        if (position.y > 2)
            neigh.emplace_back(position.x, position.y - 1);
        if (position.x + 1 < width)
            neigh.emplace_back(position.x + 1, position.y);
        if (position.y + 1 < height)
            neigh.emplace_back(position.x, position.y + 1);
        return neigh;
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
#include "field_data.h"

#endif //PROJECT_BOARD_H
