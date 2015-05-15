//
// Created by ivan on 5/7/15.
//

#include "clause.h"

Literal Clause::getBindingLiteral(Clause const &other) const {
    auto &set1 = this->getLiteralSet();
    auto it1 = set1.begin();

    auto &set2 = other.getLiteralSet();
    auto it2 = set2.begin();

    for (; it1 != set1.end(); it1++) {
        auto atom = it1->getNegated();

        for (; it2 != set2.end(); it2++) {
            if (atom < *it2) break;
            if (atom == *it2) return *it1;
        }

    }
    return Literal();
}

void Clause::print() const {
    auto cnt = _literalSet.size();
    std::cout << "( ";
    foreach(literal, _literalSet) {
        literal.print();
        if (--cnt > 0) std::cout << " or ";
    }
    std::cout << " )";
}

void Clause::addLiteral(const Literal &literal) {
    if (existsInContainer(_literalSet, literal.getNegated()))
        _isTautology = true;
    _literalSet.insert(literal);
}

void Clause::addLiteral(bool _prefix, const Atom &_label) {
    addLiteral(Literal(_prefix, _label));
}

bool Clause::operator==(const Clause &other) const {
    auto it1 = this->_literalSet.begin();
    auto it2 = other._literalSet.begin();
    if (this->_literalSet.size() != other._literalSet.size()) return false;
    for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
        if (*it1 == *it2) return true;
    }
    return false;
}

bool Clause::operator<(const Clause &other) const {
    auto it1 = this->_literalSet.begin();
    auto it2 = other._literalSet.begin();

    if (this->_literalSet.size() != other._literalSet.size())
        return this->_literalSet.size() < other._literalSet.size();

    for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
        if (*it1 < *it2) return true;
        if (*it2 < *it1) return false;
    }
    return false;
}

bool Clause::isTautology() const {
    return _isTautology;
}

bool Clause::hasLessLiteralsThan(const Clause &other) const {
    return this->getLiteralCount() < other.getLiteralCount();
}

unsigned long Clause::getLiteralCount() const {
    return _literalSet.size();
}

Clause Clause::bind(const Clause &other, const Literal &litToBind) const {
    Clause c;
    auto litNeg = litToBind.getNegated();
    foreach(literal, _literalSet) if (!(litToBind == literal))c.addLiteral(literal);
    foreach(literal, other._literalSet)if (!(litNeg == literal))c.addLiteral(literal);
    return c;
}

bool Clause::isNull() const { return getLiteralCount() == 0; }