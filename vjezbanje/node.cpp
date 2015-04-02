//
// Created by Ivan on 02-Apr-15.
//

#include "node.h"

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
    cout << string(this->getCost(), ' ') << this->getState().getId() << endl;
}

