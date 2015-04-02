//
// Created by Ivan on 02-Apr-15.
//

#include "node.h"
#include "star_trek_defs.h"

#include <iostream>

using namespace std;

bool Node::operator<(Node const &other) const {
    return this->cost < other.cost;
}

void Node::pathReconstruction(NodePtr const &leaf, stack<NodePtr> &nodeStack) {
    NodePtr node = leaf;

    while (node) {
        nodeStack.push(node);
        node = node->getParent();
    }
}

void Node::print() const {
    int x,y;
    ST::unzipCoordinates(this->getState().getId(),x,y);
    cout << string((unsigned int) this->getCost(), ' ') <<"(" <<x<<","<<y<<") " << this->getCost()<< endl;
}

