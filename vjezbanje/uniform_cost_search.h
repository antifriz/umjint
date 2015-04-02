//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_UNIFORM_COST_SEARCH_H_
#define _SEARCH_UNIFORM_COST_SEARCH_H_

#include "general_search_algorithm.h"

class UniformCostSearch : public GeneralSearchAlgorithm {
protected:

    virtual std::vector<State> const succFunct(State const s) const = 0;

    virtual bool goalFunct(State const s) const = 0;

};

#endif //_SEARCH_UNIFORM_COST_SEARCH_H_
