//
// Created by ivan on 5/7/15.
//

#include "knowledge_base.h"

void KnowledgeBase::print() const {
    foreach(clause, _clauseSet) {
        clause.print();
        std::cout << std::endl;
    }
}

std::set<Clause, RedundantComparator> &KnowledgeBase::getClauseSet() {
    return _clauseSet;
}

void KnowledgeBase::addClause(const Clause &clause) {
    while(true){
        auto it = _clauseSet.find(clause);
        if (it == _clauseSet.end()) {
            break;
        }
        else if (clause.hasLessLiteralsThan(*it)) {
            _clauseSet.erase(it);
        }
        else{
            return;
        }
    }
    _clauseSet.insert(clause);
}

bool KnowledgeBase::deduceAndAdd(Literal const &literal) {
    if (RefutationResolution::deduce(*this, literal.getNegated())) {
        addClause(literal);
        return true;
    }
    return false;
}