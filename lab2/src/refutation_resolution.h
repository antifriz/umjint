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
        KnowledgeBase<Atom> wkb(kb);
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


        std::set<Clause<Atom>, RedundantComparator<Atom>> sos;
        sos.insert(negatedConsequence);

        std::cout << "Dokazujem: ";
        negatedConsequence.getLiteralSet().begin()->getNegated().print();
        endline();

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

                eraseVector.push_back(*it2);

                sos.insert(newClause);

            }

            foreach(item, eraseVector)premiseSet.erase(item);

            wkb.addClause(clause1);
            //printMe(sos.size());
        }
        // printMe("Nije istina");
        return false;
    }


};


#endif //PROJECT_REFUTATION_RESOLUTION_H
