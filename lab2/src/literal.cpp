//
// Created by ivan on 5/7/15.
//

#include "literal.h"

void Literal::togglePrefix() { _prefix = !_prefix; }

Literal Literal::getNegated() const {
    Literal copied(*this);
    copied.togglePrefix();
    return copied;
}

bool Literal::operator==(const Literal &other) const {
    return _atom == other._atom && _prefix == other._prefix;
}

bool Literal::operator<(const Literal &other) const {
    return _atom == other._atom ? _prefix < other._prefix : _atom < other._atom;
}

void Literal::print() const {

    if (!_prefix)
        std::cout << "( not ";

    _atom.print();
    if (!_prefix) std::cout << " )";
}

bool Literal::isNull() { return _null; }