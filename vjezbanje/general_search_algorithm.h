//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_GENERAL_SEARCH_ALGORITHM_H_
#define _SEARCH_GENERAL_SEARCH_ALGORITHM_H_

#include "node.h"

#include <forward_list>
#include <vector>


class GeneralSearchAlgorithm {
private:
    std::forward_list<NodePtr> const expand(NodePtr const parent);

    inline NodePtr generateInitialNode(State const &state);

protected:
    virtual NodePtr generateNode(NodePtr const &parent, State const &state) = 0;

    virtual std::vector<State> succFunct(State const s) = 0;

    virtual bool goalFunct(State const s) = 0;

    virtual bool nodePtrComparator(NodePtr a, NodePtr b) = 0;

public:
    GeneralSearchAlgorithm() {
    }

    NodePtr search(State initialState);
};


#endif //_SEARCH_GENERAL_SEARCH_ALGORITHM_H_
