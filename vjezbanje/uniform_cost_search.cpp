//
// Created by Ivan on 02-Apr-15.
//

#include "uniform_cost_search.h"

using namespace std;

NodePtr UniformCostSearch::generateNode(NodePtr const &parent, State const &state) const {
    return make_shared<Node>(state, parent->getCost() + getDistance(parent->getState(), state), parent);
}
