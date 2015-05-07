//
// Created by ivan on 5/6/15.
//

#ifndef PROJECT_REFUTATION_RESOLUTION_H
#define PROJECT_REFUTATION_RESOLUTION_H

//
// Created by ivan on 5/6/15.
//

#include <string>
#include <vector>
#include <iomanip>
#include <queue>

#include <boost/algorithm/string.hpp>

#include <fstream>


#include <set>
#include <ostream>
#include <iostream>
#include <bits/stl_queue.h>
#include "point_atom.h"
#include "field_data.h"
#include "board.h"
#include "literal.h"
#include "clause.h"
#include "knowledge_base.h"


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
                auto newClause = clause1.prepareForBind(clause2);
                if (newClause.getLiteralCount() == clause1.getLiteralCount()) continue;

                newClause.bind(clause2);

                if (newClause.isTautology()) continue;

                if (newClause.isNull()) return true;

                auto it = premiseSet.find(newClause);

                if (it == premiseSet.end()) {
                    sos.push(newClause);
                    continue;
                }

                if (newClause.hasLessLiteralsThan(*it)) {
                    premiseSet.erase(it);
                    sos.push(newClause);
                };
            }

            wkb.addClause(clause1);
        }
        return false;
    }


};




#endif //PROJECT_REFUTATION_RESOLUTION_H
