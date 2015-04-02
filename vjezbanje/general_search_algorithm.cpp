//
// Created by Ivan on 02-Apr-15.
//

#include "general_search_algorithm.h"

#include <queue>
#include <set>
#include <iostream>

using namespace std;


NodePtr GeneralSearchAlgorithm::generateInitialNode(State const &state) const {
    return std::make_shared<Node>(state, 0, NodePtr());
}

forward_list<NodePtr> const GeneralSearchAlgorithm::expand(NodePtr const parent) const {
    vector<State> container = succFunct(parent->getState());
    forward_list<NodePtr> succQueue;

    for (vector<State>::iterator it = container.begin(); it != container.end(); ++it) {
        succQueue.push_front(generateNode(parent, *it));
    }

    //cout<<"expand: "<<distance(begin(succQueue),end(succQueue))<<endl;

    return succQueue;
}


NodePtr GeneralSearchAlgorithm::search(State initialState) const {
    priority_queue<NodePtr,vector<NodePtr>,GeneralSearchAlgorithm::Comparator> openQueue;
    openQueue.push(generateInitialNode(initialState));

    set<State> discoveredSet;

    discoveredSet.insert(initialState);

    while (!openQueue.empty()) {
        NodePtr n = openQueue.top();
        openQueue.pop();

        n->print();
        //cout<<"size: "<<openQueue.size()<<endl;

        if (goalFunct(n->getState())) return n;

        forward_list<NodePtr> container = expand(n);
        for (forward_list<NodePtr>::iterator it = container.begin(); it != container.end(); ++it) {
            Node &item = **it;
            if (discoveredSet.find(item.getState()) == discoveredSet.end()) {
                discoveredSet.insert(item.getState());
                /*cout<<"pushed ";
                (*it)->print();*/
                openQueue.push(*it);
            }
        }
    }
    return nullptr;
}
