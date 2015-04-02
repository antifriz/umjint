//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_UNIFORM_COST_SEARCH_H_
#define _SEARCH_UNIFORM_COST_SEARCH_H_

#include "general_search_algorithm.h"

class UniformCostSearch : public GeneralSearchAlgorithm {
protected:

    virtual std::vector<State> succFunct(State const s) = 0;

    virtual bool goalFunct(State const s) = 0;

    virtual NodePtr generateNode(NodePtr const &parent, State const &state);

    virtual bool nodePtrComparator(NodePtr a, NodePtr b);


};

#endif //_SEARCH_UNIFORM_COST_SEARCH_H_
