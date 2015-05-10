//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_CLAUSE_H
#define PROJECT_CLAUSE_H

#include <set>
#include "main.h"
#include "literal.h"


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

    inline const std::set<Literal<Atom>> &getLiteralSet() const { return _literalSet; }

    template<typename... Args>
    Clause(bool _prefix, const Atom &_label, Args... args) : Clause(args...) {
        addLiteral(_prefix, _label);
    }

    Literal<Atom> getBindingLiteral(Clause<Atom> const &other) const {
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
        return Literal<Atom>();
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
        if (this->_literalSet.size() != other._literalSet.size()) return false;
        for (; it1 != this->_literalSet.end() && it2 != other._literalSet.end(); it1++, it2++) {
            if (*it1 == *it2) return true;
        }
        return false;
    }


    bool operator<(const Clause &other) const {
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

    bool isTautology() const {
        return _isTautology;
    }

    bool hasLessLiteralsThan(const Clause &other) const {
        return this->getLiteralCount() < other.getLiteralCount();
    }

    unsigned long getLiteralCount() const {
        return _literalSet.size();
    }

    Clause bind(const Clause<Atom> &other, const Literal<Atom> &litToBind) const {
        Clause<Atom> c;
        auto litNeg = litToBind.getNegated();
        foreach(literal, _literalSet) if (!(litToBind == literal))c.addLiteral(literal);
        foreach(literal, other._literalSet)if (!(litNeg == literal))c.addLiteral(literal);
        return c;
    }

    bool isNull() const { return getLiteralCount() == 0; }
};


#endif //PROJECT_CLAUSE_H
