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
    std::forward_list<NodePtr> const expand(NodePtr const parent) const;

    inline NodePtr generateInitialNode(State const &state) const;


protected:
    virtual NodePtr generateNode(NodePtr const &parent, State const &state) const = 0;

    virtual std::vector<State> succFunct(State const s) const = 0;

    virtual bool goalFunct(State const s) const = 0;

public:
    GeneralSearchAlgorithm() {
    }

    class Comparator {
    public:
        inline bool operator()(NodePtr a, NodePtr b) const {
            return a->getCost() > b->getCost();
        }
    };

    virtual NodePtr search(State initialState) const;
};


#endif //_SEARCH_GENERAL_SEARCH_ALGORITHM_H_
