//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_LITERAL_H
#define PROJECT_LITERAL_H

template<typename Atom>
class Literal final {
private:
    bool _prefix;
    Atom _atom;
public:

    Literal(bool _prefix, const Atom &_atom) : _prefix(_prefix), _atom(_atom) { }

    inline void togglePrefix() { _prefix ^= 1; }

    Literal getNegated() const {
        Literal copied(*this);
        copied.togglePrefix();
        return copied;
    }

    bool operator==(const Literal &other) const {
        return _atom == other._atom && _prefix == other._prefix;
    }

    bool operator<(const Literal &other) const {
        return _atom == other._atom ? _prefix < other._prefix : _atom < other._atom;
    }

    void print() const {
        if (!_prefix) std::cout << "( not ";
        _atom.print();
        if (!_prefix) std::cout << " )";
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

#endif //PROJECT_LITERAL_H
