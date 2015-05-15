//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_KNOWLEDGE_BASE_H
#define PROJECT_KNOWLEDGE_BASE_H

#include <iostream>
#include "lazy_defines.h"
#include "clause.h"
#include "refutation_resolution.h"
#include "redundant_comparator.h"

class KnowledgeBase {
private:
    std::set<Clause, RedundantComparator> _clauseSet;

public:

    void print() const;

    KnowledgeBase() { }

    std::set<Clause, RedundantComparator> & getClauseSet();

    void addClause(const Clause &clause);

    bool deduceAndAdd(Literal const &literal);

};


#endif //PROJECT_KNOWLEDGE_BASE_H
