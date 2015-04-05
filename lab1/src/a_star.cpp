//
// Created by Ivan on 02-Apr-15.
//

#include <iostream>
#include "a_star.h"

using namespace std;


NodePtr GeneralSearchAlgorithm::search(int initialStateId) {
    set<NodePtr, Comparator> open;
    set<NodePtr> closed;

    open.insert(generateInitialNode(initialStateId));

    for (openNodeCount = 0; !open.empty(); ++openNodeCount) {
        NodePtr n = removeHead(open);

        if (goalFunc(n->getState())) return n;
        closed.insert(n);

        for (auto& m: expand(n))
            if (!seenBetterState(m, open) && !seenBetterState(m, closed))
                open.insert(m);
    }

    return nullptr;
}

template<typename ComparatorT>
bool GeneralSearchAlgorithm::seenBetterState(NodePtr const& n, std::set<NodePtr, ComparatorT>& container) {

    auto it = container.begin();
    for (; it != container.end(); ++it)
        if ((*it)->getState().getId() == n->getState().getId())
            break;

    if (it == container.end())
        return false;

    if ((*it)->getCurrentCost() <= n->getCurrentCost())
        return true;

    container.erase(it);
    return false;
}

vector<NodePtr> GeneralSearchAlgorithm::expand(NodePtr const& parent) {
    vector<NodePtr> succQueue;

    for (auto& s: succFunc(parent->getState()))
        succQueue.push_back(generateNode(parent, s));

    return succQueue;
}

template<typename ComparatorT>
const NodePtr GeneralSearchAlgorithm::removeHead(set<NodePtr, ComparatorT>& container) {
    auto it = container.end();
    --it;
    auto val = *it;
    container.erase(it);
    return val;
}


