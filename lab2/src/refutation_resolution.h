//
// Created by ivan on 5/6/15.
//

#ifndef PROJECT_REFUTATION_RESOLUTION_H
#define PROJECT_REFUTATION_RESOLUTION_H

//
// Created by ivan on 5/6/15.
//



#include "knowledge_base.h"
#include "main.h"
#include "point_atom.h"

#include <queue>

template<typename Atom>
class KnowledgeBase;

template<typename Atom>
class RefutationResolution {

public:
    static bool deduce(const KnowledgeBase<Atom> &kb, const Clause<Atom> &negatedConsequence) {

        KnowledgeBase<Atom> wkb(kb);

        auto premiseSet = wkb.getClauseSetCopy();


        std::queue<Clause<Atom>> sos;
        sos.push(negatedConsequence);

        while (!sos.empty()) {
            Clause<Atom> clause1 = sos.back();
            sos.pop();

            foreach(clause2, premiseSet) {
                auto literal = clause1.getBindingLiteral(clause2);

                if (literal.isNull()) continue;

                auto newClause = clause1.bind(clause2, literal);

                if (newClause.isTautology()) continue;


                if (newClause.isNull()) return true;

                auto it = premiseSet.find(newClause);

                if (it == premiseSet.end()) {
                    sos.push(newClause);
                    continue;
                }

                if (!newClause.hasLessLiteralsThan(*it)) continue;

                premiseSet.erase(it);
                sos.push(newClause);

            }

            wkb.addClause(clause1);
        }
        // printMe("Nije istina");
        return false;
    }

};


#endif //PROJECT_REFUTATION_RESOLUTION_H
