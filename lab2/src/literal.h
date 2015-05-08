//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_LITERAL_H
#define PROJECT_LITERAL_H

#include <iostream>
#include "main.h"

template<typename Atom>
class Literal final {
private:
    bool _prefix;
    Atom _atom;
    bool _null;
public:

    Literal() : _null(true) { }

    Literal(bool _prefix, const Atom &_atom) : _prefix(_prefix), _atom(_atom), _null(false) { }

    void togglePrefix() { _prefix = !_prefix; }

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

        if (!_prefix)
            std::cout << "( not ";

        _atom.print();
        if (!_prefix) std::cout << " )";
    }

    bool isNull() { return _null; }
};


#endif //PROJECT_LITERAL_H
