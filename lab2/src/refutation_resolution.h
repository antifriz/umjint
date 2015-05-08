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

struct RedundantComparator {
    bool operator()(Clause<Atom> const &a, Clause<Atom> const &other) {
        auto it1 = a.getLiteralSet().begin();
        auto it2 = other.getLiteralSet().begin();
        for (; it1 != a.getLiteralSet().end() && it2 != other.getLiteralSet().end(); it1++, it2++) {
            if (*it1 < *it2) return true;
            if (*it2 < *it1) return false;
        }
        return true;
    }
};


template<typename Atom>
class RefutationResolution {
public:
    static bool deduce(const KnowledgeBase<Atom> &kb, const Clause<Atom> &negatedConsequence) {
        std::set<Clause<Atom>> premiseSet(kb.getClauseSet());
/*

        foreach(item,){
            auto it = premiseSet.find(item);
            if(it != premiseSet.end()){
                if(it->getLiteralCount()<=item.getLiteralCount()) continue;
                premiseSet.erase(it);
            }
            premiseSet.insert(item);
        }
*/

        kb.print();
        endline();

        std::set<Clause<Atom>, RedundantComparator<Atom>> sos;
        sos.insert(negatedConsequence);

        std::cout << "Dokazujem: ";
        negatedConsequence.getLiteralSet().begin()->getNegated().print();
        endline();

        while (!sos.empty()) {
            auto it = sos.end();
            Clause<Atom> clause1(*(--it));
            sos.erase(it);

            foreach(clause2, premiseSet) {
                auto literal = clause1.getBindingLiteral(clause2);

                if (literal.isNull())continue;

                auto newClause = clause1.bind(clause2, literal);
/*                std::cout << std::endl << " > ";
                clause1.print();
                std::cout << std::endl << " + ";
                clause2.print();
                std::cout << std::endl << " = ";
                newClause.print();
                endline();*/

                if (newClause.isTautology()) continue;
                //printMe("nije tautologija");


                if (newClause.isNull()) {
                    //  printMe("Istina");
                    return true;
                }
                //printMe("nije null");

                auto it2 = premiseSet.find(newClause);

                if (it2 == premiseSet.end()) {
                    sos.insert(newClause);
                    //newClause.print();endline();
                    //printMe("ne postoji");
                    continue;
                }

                if (!newClause.hasLessLiteralsThan(*it2)) continue;
                //printMe("ima manje literala");

                premiseSet.erase(it2);
                sos.insert(newClause);

            }

            premiseSet.insert(clause1);
            //printMe(sos.size());
        }
        // printMe("Nije istina");
        return false;
    }


};


#endif //PROJECT_REFUTATION_RESOLUTION_H
