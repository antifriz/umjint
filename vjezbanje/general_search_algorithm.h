//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_GENERAL_SEARCH_ALGORITHM_H_
#define _SEARCH_GENERAL_SEARCH_ALGORITHM_H_

#include "node.h"

#include <set>


class GeneralSearchAlgorithm {

private:
    long openNodeCount;

    inline NodePtr generateInitialNode(int id) const {
        return std::make_shared<Node>(State(id), 0, 0, NodePtr());
    }


    inline NodePtr generateNode(NodePtr const &parent, State const &state) const {
        auto currentCost = parent->getCurrentCost() + distanceFunc(parent->getState(), state);
        return std::make_shared<Node>(state, currentCost, currentCost + heuristicFunc(state), parent);
    };

    std::vector<NodePtr> expand(NodePtr const &parent);

    template<typename ComparatorT>
    const NodePtr removeHead(std::set<NodePtr, ComparatorT> &container);

    template<typename ComparatorT>
    bool seenBetterState(NodePtr const &n, std::set<NodePtr, ComparatorT> &container);

protected:

    virtual int distanceFunc(State const &a, State const &b) const = 0;

    virtual int heuristicFunc(State const &s) const = 0;

    virtual const std::vector<State> succFunc(State const &state) const = 0;

    virtual bool goalFunc(State const &state) const = 0;

    inline long getOpenNodeCount() const {
        return openNodeCount;
    }

    NodePtr search(int initialStateId);

public:
    class Comparator {
    public:
        inline bool operator()(NodePtr a, NodePtr b) const {
            return *b < *a;
        }
    };

    GeneralSearchAlgorithm() : openNodeCount(0) {
    }

};


#endif //_SEARCH_GENERAL_SEARCH_ALGORITHM_H_
