//
// Created by ivan on 5/15/15.
//

#ifndef PROJECT_REDUNDANT_COMPARATOR_H
#define PROJECT_REDUNDANT_COMPARATOR_H


#include "literal.h"
#include "clause.h"

struct RedundantComparator {
    bool operator()(Clause const &a, Clause const &b) {
        std::set<Literal> aSet(a.getLiteralSet());
        std::set<Literal> bSet(b.getLiteralSet());

        auto it1 = aSet.begin();
        auto it2 = bSet.begin();
        for (; it1 != aSet.end() && it2 != bSet.end(); it1++, it2++) {
            if (*it1 < *it2) return true;
            if (*it2 < *it1) return false;
        }
        return false;
    }
};


#endif //PROJECT_REDUNDANT_COMPARATOR_H
