#include "refutation_resolution.h"
#include "redundant_comparator.h"

bool  RefutationResolution::deduce(const KnowledgeBase &kb, const Clause &negatedConsequence) {
    std::cout << "Dokazujem (negirano): ";
    negatedConsequence.print();
    endline();

    KnowledgeBase wkb(kb);

    std::set<Clause, RedundantComparator> sos;
    sos.insert(negatedConsequence);

    while (!sos.empty()) {

        auto it = sos.end();
        Clause clause1(*(--it));
        sos.erase(it);

        std::set<Clause, RedundantComparator> premiseSet = wkb.getClauseSet();

        std::vector<Clause> eraseVector;

        foreach(clause2, premiseSet) {

            auto literal = clause1.getBindingLiteral(clause2);

            if (literal.isNull())continue;

            auto newClause = clause1.bind(clause2, literal);

            if (newClause.isTautology()) continue;

            if (newClause.isNull()) {
                printMe("dokazano");
                return true;
            }

            auto it2 = premiseSet.find(newClause);

            if (it2 == premiseSet.end()) {
                sos.insert(newClause);
                continue;
            }

            if (!newClause.hasLessLiteralsThan(*it2)) continue;

            eraseVector.push_back(*it2);

            sos.insert(newClause);
        }

        foreach(item, eraseVector)premiseSet.erase(item);

        wkb.addClause(clause1);
    }
    printMe("nije dokazano");
    return false;
}