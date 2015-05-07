//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_KNOWLEDGE_BASE_H
#define PROJECT_KNOWLEDGE_BASE_H

#include <iostream>
#include "main.h"
#include "clause.h"
#include "refutation_resolution.h"

template<typename Atom>
class RefutationResolution;

template<typename Atom>
class KnowledgeBase {
private:
    std::set<Clause<Atom>> _clauseSet;

public:

    void print() const {
        foreach(clause, _clauseSet) {
            clause.print();
            std::cout << std::endl;
        }
    }

    KnowledgeBase() { }

    std::set<Clause<Atom>> getClauseSetCopy() const {
        return _clauseSet;
    }

    void addClause(const Clause<Atom> &clause) {
        auto it = _clauseSet.find(clause);

        if (it != _clauseSet.end() && it->hasLessLiteralsThan(clause))
            return;

        _clauseSet.insert(clause);
    }


    template<typename... Args>
    void addClause(const Clause<Atom> &clause, Args... args) {
        addClause(clause);
        addClause(args...);
    }


    bool clauseExists(const Clause<Atom> &clause) {
        return _clauseSet.find(clause) != _clauseSet.end();
    }


    void deduceAndAdd(const Atom &atom) {
        auto negClauseT = Clause<Atom>(true, atom);
        auto negClauseF = Clause<Atom>(false, atom);
        if (RefutationResolution<Atom>::deduce(*this, negClauseT))
            addClause(negClauseT);
        else if (RefutationResolution<Atom>::deduce(*this, negClauseF))
            addClause(negClauseF);
    }
};


#endif //PROJECT_KNOWLEDGE_BASE_H
