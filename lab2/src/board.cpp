//
// Created by ivan on 5/7/15.
//

#include "board.h"

void Board::print() const {
    ntimesy(height)ntimesx(width) {
        _fields[height * y + x].print();
        if (x == width - 1) std::cout << std::endl;
    }
}

std::vector<Point> Board::getAllPoints() {
    std::vector<Point> v;
    ntimesx(height)ntimesy(width)v.push_back(Point(x, y));
    return v;
}

void Board::load(std::string path) {
    std::string line;
    std::ifstream file(path);
    if (!file.is_open()) throw "Unable to read file";

    width = 0;
    height = 0;
    while (getline(file, line)) {
        std::vector<std::string> strParts;
        split(strParts, line, boost::algorithm::is_any_of(" "));

        std::vector<std::string> coordinates;
        split(coordinates, strParts[0], boost::algorithm::is_any_of(","));
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
    return _fields.at((unsigned long) ((point.x - 1) * height + point.y - 1));
}