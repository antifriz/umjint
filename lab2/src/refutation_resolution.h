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
#include <assert.h>
#include "boost/date_time/posix_time/posix_time.hpp"

template<typename Atom>
class KnowledgeBase;

template<typename Atom>

struct RedundantComparator {
    bool operator()(Clause<Atom> const &a, Clause<Atom> const &b) {
        std::set<Literal<Atom>> aSet(a.getLiteralSet());
        std::set<Literal<Atom>> bSet(b.getLiteralSet());

        auto it1 = aSet.begin();
        auto it2 = bSet.begin();
        for (; it1 != aSet.end() && it2 != bSet.end(); it1++, it2++) {
            if (*it1 < *it2) return true;
            if (*it2 < *it1) return false;
        }
        return false;
    }
};


template<typename Atom>
class RefutationResolution {
public:
    static bool deduce(const KnowledgeBase<Atom> &kb, const Clause<Atom> &negatedConsequence) {
        std::cout << "Dokazujem (negirano): ";
        negatedConsequence.print();
        endline();

        KnowledgeBase<Atom> wkb(kb);

        bool nec = *negatedConsequence.getLiteralSet().begin()==Literal<Atom>(false,Atom(Pit,Point(3,3)));

        std::set<Clause<Atom>, RedundantComparator<Atom>> sos;
        sos.insert(negatedConsequence);

        while (!sos.empty()) {

            auto it = sos.end();
            Clause<Atom> clause1(*(--it));
            sos.erase(it);

            std::set<Clause<Atom>, RedundantComparator<Atom>> premiseSet = wkb.getClauseSet();

            std::vector<Clause<Atom>> eraseVector;

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


};


#endif //PROJECT_REFUTATION_RESOLUTION_H
