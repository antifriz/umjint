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
            /*            foreach(item, premiseSet) {
                           item.print();
                           endline();
                       }
                       endline();
                       endline();
                       clause1.print();
                       endline();
                       endline();*/

            foreach(clause2, premiseSet) {
                auto literal = clause1.prepareForBind(clause2);

                if (literal.isNull()) continue;
                printMe("hej");

                auto newClause = clause1.bind(clause2, literal);
                std::cout << std::endl << " > ";
                clause1.print();
                std::cout << std::endl << " + ";
                clause2.print();
                std::cout << std::endl << " = ";
                newClause.print();
                endline();

                if (newClause.isTautology()) continue;
                printMe("nije tautologija");


                if (newClause.isNull()) {
                    printMe("Istina");
                    return true;
                }
                printMe("nije null");

                auto it = premiseSet.find(newClause);

                if (it == premiseSet.end()) {

                    sos.push(newClause);
                    printMe("ne postoji");

                    continue;
                }

                if (!newClause.hasLessLiteralsThan(*it)) continue;
                printMe("ima manje literala");

                premiseSet.erase(it);
                sos.push(newClause);

            }

            wkb.addClause(clause1);
        }
        printMe("Nije istina");
        return false;
    }

};


#endif //PROJECT_REFUTATION_RESOLUTION_H
