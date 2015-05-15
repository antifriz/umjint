//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_CLAUSE_H
#define PROJECT_CLAUSE_H

#include <set>
#include "lazy_defines.h"
#include "literal.h"


class Clause final {
private:
    bool _isTautology = false;


    std::set<Literal> _literalSet;

public:


    Clause() { }

    Clause(const Literal &literal) {
        _literalSet.insert(literal);
    }

    Clause(bool _prefix, const Atom &_label) : Clause(Literal(_prefix, _label)) {
    }

    inline const std::set<Literal> &getLiteralSet() const { return _literalSet; }

    template<typename... Args>
    Clause(bool _prefix, const Atom &_label, Args... args) : Clause(args...) {
        addLiteral(_prefix, _label);
    }

    Literal getBindingLiteral(Clause const &other) const;

    template<typename... Args>
    Clause(const Literal &first, Args... args) : Clause(args...) {
        addLiteral(first);
    }

    void print() const;

    void addLiteral(const Literal &literal);


    void addLiteral(bool _prefix, const Atom &_label);

    bool operator==(const Clause &other) const;


    bool operator<(const Clause &other) const;

    bool isTautology() const;

    bool hasLessLiteralsThan(const Clause &other) const;

    unsigned long getLiteralCount() const;

    Clause bind(const Clause &other, const Literal &litToBind) const;

    bool isNull() const;
};


#endif //PROJECT_CLAUSE_H
