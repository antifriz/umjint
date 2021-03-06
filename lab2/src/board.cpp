//
// Created by ivan on 5/7/15.
//

#include <fstream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "board.h"

void Board::print() const {
    ntimesy(height)ntimesx(width) {
            _fields[height * x + y].print();
            if (x == width - 1) std::cout << std::endl;
        }
}

void Board::print(std::set<Point> visited, Point current) const {
    ntimesy(height)ntimesx(width) {
            assert(visited.find(current)==visited.end());

            if (visited.find(Point(x + 1, y + 1)) != visited.end())
                std::cout << "X ";
            else if(Point(x + 1, y + 1) == current)
                std::cout<< "O ";
            else
                _fields[height * x + y].print();
            if (x == width - 1) std::cout << std::endl;
        }
}

void Board::load(std::string path) {
    std::string line;
    std::ifstream file(path);
    if (!file.is_open()) throw "Unable to read file";

    width = 0;
    height = 0;
    while (getline(file, line)) {
        std::vector<std::string> strParts;
        boost::algorithm::split(strParts, line, boost::algorithm::is_any_of(" "));

        std::vector<std::string> coordinates;
        boost::algorithm::split(coordinates, strParts[0], boost::algorithm::is_any_of(","));
        int x = stoi(coordinates[0].substr(1));
        int y = stoi(coordinates[1]);

        width = std::max(width, x);
        height = std::max(height, y);

        _fields.emplace_back(strParts[1].back() == '1', strParts[2].back() == '1', strParts[3].back() == '1',
                             strParts[4].back() == '1', strParts[5].back() == '1', strParts[6].back() == '1');
    }
    file.close();
}

const FieldData &Board::getData(const Point &point) const {
    assert(point.x >= 1);
    assert(point.y >= 1);
    assert(point.y <= width);
    assert(point.y <= height);
    auto val = ((point.x - 1) * height + point.y - 1);
    return _fields.at((unsigned long) val);
}

std::vector<Point> Board::getNeighbours(const Point &position) const {
    std::vector<Point> neigh;
    if (position.x > 1)
        neigh.emplace_back(position.x - 1, position.y);
    if (position.y > 1)
        neigh.emplace_back(position.x, position.y - 1);
    if (position.x < width)
        neigh.emplace_back(position.x + 1, position.y);
    if (position.y < height)
        neigh.emplace_back(position.x, position.y + 1);
    return neigh;
}