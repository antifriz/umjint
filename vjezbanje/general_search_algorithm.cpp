//
// Created by Ivan on 02-Apr-15.
//

#include <iostream>
#include "general_search_algorithm.h"

using namespace std;


NodePtr GeneralSearchAlgorithm::search(int initialStateId) {

    set<NodePtr, Comparator> open;
    open.insert(generateInitialNode(initialStateId));

    set<NodePtr> closed;

    while (!open.empty()) {
/*
        for (auto& p:open) {
            int x, y;
            ST::unzipCoordinates(p->getState().getId(), x, y);
            cout << "(" << x << "," << y << ")(" << p->getEstimatedCost() << "), ";
        }
        cout << endl;
*/

        NodePtr n = removeHead(open);

        openNodeCount++;

        n->print();
        //cout<<"size: "<<openQueue.size()<<endl;

        if (goalFunc(n->getState())) return n;

        closed.insert(n);
/*
        cout << "closed:" << n->getState().getId() << endl;
*/

        for (auto& m: expand(n)) {
            /*
            {
                int x, y;
                ST::unzipCoordinates(m->getState().getId(), x, y);
                cout << "expanded:" << "(" << x << "," << y << ")" << endl;
            }
            if (closed.find(m) != closed.end()) {
                cout << "seen" << endl;
                continue;
            };*/

            if (seenBetterState(m, open) || seenBetterState(m, closed)) {
                //cout << "seen" << endl;
                continue;
            };

            open.insert(m);
/*
            for (auto& p:open) {
                int x, y;
                ST::unzipCoordinates(p->getState().getId(), x, y);
                cout << "(" << x << "," << y << ")(" << p->getEstimatedCost() << "), ";
            }
            cout << endl;*/
        }
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


