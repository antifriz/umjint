//
// Created by Ivan on 02-Apr-15.
//

#include "ST_uniform_cost_search.h"

using namespace std;

std::vector<State> ST_uniform_cost_search::succFunct(State const s) {//TODO
    vector<State> container;

    container.push_back(State(s.getId() * 2));

    container.push_back(State(s.getId() * 2 + 1));
    return container;
}


bool ST_uniform_cost_search::goalFunct(State const s) {//TODO
    return s.getId() > 63;
}