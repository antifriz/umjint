//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
#define _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_


#include "uniform_cost_search.h"

class ST_uniform_cost_search : public UniformCostSearch {

    virtual std::vector<State> succFunct(State const s);

    virtual bool goalFunct(State const s);
};


#endif //_SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
