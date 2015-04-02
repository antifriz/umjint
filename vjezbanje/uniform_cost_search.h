//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_UNIFORM_COST_SEARCH_H_
#define _SEARCH_UNIFORM_COST_SEARCH_H_

#include "general_search_algorithm.h"

class UniformCostSearch : public GeneralSearchAlgorithm {
protected:

    virtual std::vector<State> succFunct(State const s) const = 0;

    virtual bool goalFunct(State const s) const = 0;

    virtual int getDistance(State const &a, State const &b) const = 0;

    virtual NodePtr generateNode(NodePtr const &parent, State const &state) const;


};

#endif //_SEARCH_UNIFORM_COST_SEARCH_H_
