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
    std::set<Clause<Atom>, RedundantComparator<Atom>> _clauseSet;

public:

    void print() const {
        foreach(clause, _clauseSet) {
            clause.print();
            std::cout << std::endl;
        }
    }

    KnowledgeBase() { }

    std::set<Clause<Atom>, RedundantComparator<Atom>> &getClauseSet() {
        return _clauseSet;
    }

    void addClause(const Clause<Atom> &clause) {
        auto it = _clauseSet.find(clause);

/*        endline();endline();
        printMe("Knlg");
        print();endline();
        printMe("Cls");
        clause.print();endline();*/

        if (it == _clauseSet.end()) {
            _clauseSet.insert(clause);
        } else if (clause.hasLessLiteralsThan(*it)) {

            _clauseSet.erase(it);
            _clauseSet.insert(clause);
        } else {

        }
        /*
 *
        if (clause.getLiteralCount() == 1)

            iterate(_clauseSet)
                iterate2(_clauseSet) {

                }
                            if(it2->getLiteralCount()== 1)

                        if (literal == *clause.getLiteralSet().begin()) continue;

                            hasAll = false;
                            break;

                    }
                    if (hasAll)
                        _clauseSet.erase(it);
                    if(hasAll)
                        printMe("jej");
                }*/

    }


    template<typename... Args>
    void addClause(const Clause<Atom> &clause, Args... args) {
        addClause(clause);
        addClause(args...);
    }


    bool clauseExists(const Clause<Atom> &clause) {
        foreach(item, _clauseSet)if (item == clause)
                return true;
        return false;
    }


    void deduceAndAdd(const Atom &atom) {
        auto negClauseT = Clause<Atom>(true, atom);
        auto negClauseF = Clause<Atom>(false, atom);
        if (RefutationResolution<Atom>::deduce(*this, negClauseT))
            addClause(negClauseF);
        else if (RefutationResolution<Atom>::deduce(*this, negClauseF))
            addClause(negClauseT);
    }
};


#endif //PROJECT_KNOWLEDGE_BASE_H
