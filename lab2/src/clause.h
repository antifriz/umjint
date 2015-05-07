//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_CLAUSE_H
#define PROJECT_CLAUSE_H

template<typename Atom>
class Clause final {
private:
    bool _isTautology = false;

    std::set<Literal<Atom>> _literalSet;


public:

    Clause() { }

    Clause(const Literal<Atom> &literal) {
        _literalSet.insert(literal);
    }

    Clause(bool _prefix, const Atom &_label) : Clause(Literal<Atom>(_prefix, _label)) {
    }


    template<typename... Args>
    Clause(bool _prefix, const Atom &_label, Args... args) : Clause(args...) {
        addLiteral(_prefix, _label);
    }

    Clause prepareForBind(Clause<Atom> const &other) {
        auto it1 = _literalSet.begin();
        auto it2 = other._literalSet.begin();
        for (; it1 != _literalSet.end(); it1++) {
            auto atom = it1->getNegated();

            for (; it2 != _literalSet.end(); it2++) {
                if (atom < *it2) break;
                if (atom == *it2) {
                    Clause c(*this);
                    c._literalSet.erase(it1);
                    return c;
                };
            }

        }

        return *this;
    }

    template<typename... Args>
    Clause(const Literal<Atom> &first, Args... args) : Clause(args...) {
        addLiteral(first);
    }

    void print() const {
        auto cnt = _literalSet.size();
        std::cout << "( ";
        foreach(literal, _literalSet) {
            literal.print();
            if (--cnt > 0) std::cout << " or ";
        }
        std::cout << " )";
    }

    void addLiteral(const Literal<Atom> &literal) {
        if (existsInContainer(_literalSet, literal.getNegated()))
            _isTautology = true;
        _literalSet.insert(literal);
    }

    void addLiteral(bool _prefix, const Atom &_label) {
        addLiteral(Literal<Atom>(_prefix, _label));
    }

    bool operator==(const Clause &other) const {
        auto it1 = this->_literalSet.begin();
        auto it2 = other._literalSet.begin();
        for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
            if (*it1 == *it2) return true;
        }
        return false;
    }


    bool operator<(const Clause &other) const {
        auto it1 = this->_literalSet.begin();
        auto it2 = other._literalSet.begin();
        for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
            if (*it1 < *it2) return true;
            if (*it2 < *it1) return false;
        }
        return false;
    }

    bool isTautology() const {
        return _isTautology;
    }

    bool hasLessLiteralsThan(const Clause &other) const {
        return this->_literalSet.size() < other._literalSet.size();
    }

    unsigned long getLiteralCount() const {
        return _literalSet.size();
    }

    Clause<Atom> bind(const Clause<Atom> &other) const {
        Clause<Atom> c(*this);
        foreach(literal, other._literalSet)c.addLiteral(literal);
        return c;
    }

    bool isNull() const { return getLiteralCount() == 0; };
};

#include <string>
#include <vector>
#include <iomanip>
#include <queue>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <set>
#include <iostream>
#include "literal.h"

#endif //PROJECT_CLAUSE_H
