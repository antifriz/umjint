//
// Created by Ivan on 02-Apr-15.
//

#include "uniform_cost_search.h"

using namespace std;

bool UniformCostSearch::nodePtrComparator(NodePtr a, NodePtr b) {
    return a->getCost() > b->getCost();
}

NodePtr UniformCostSearch::generateNode(NodePtr const &parent, State const &state) {
    return make_shared<Node>(state, parent->getCost() + 1, parent);
}