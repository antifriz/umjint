//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_LITERAL_H
#define PROJECT_LITERAL_H

#include <iostream>
#include "lazy_defines.h"
#include "atom.h"

class Literal final {
private:
    bool _prefix;
    Atom _atom;
    bool _null;
public:

    Literal() : _null(true) { }

    Literal(bool _prefix, const Atom &_atom) : _prefix(_prefix), _atom(_atom), _null(false) { }

    void togglePrefix();

    Literal getNegated() const;

    bool operator==(const Literal &other) const;

    bool operator<(const Literal &other) const;

    void print() const;

    bool isNull();
};


#endif //PROJECT_LITERAL_H
