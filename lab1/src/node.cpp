//
// Created by Ivan on 02-Apr-15.
//

#include "node.h"
#include "star_trek_defs.h"

#include <iostream>

using namespace std;

bool Node::operator<(Node const &other) const {
    if (this->getEstimatedCost() != other.getEstimatedCost())
        return this->getEstimatedCost() < other.getEstimatedCost();
    else
        return this->getState() < other.getState();
}

void Node::pathReconstruction(NodePtr const &leaf, stack<NodePtr> &nodeStack) {
    NodePtr node = leaf;

    while (node) {
        nodeStack.push(node);
        node = node->getParent();
    }
}
/*
void Node::print_w_depth() const {
    int x, y;
    ST::unzipCoordinates(this->getState().getId(), x, y);
    cout << string((unsigned int) this->getCurrentCost(), ' ') << "(" << y + 1 << "," << x + 1 << ") " << this->getCurrentCost() << endl;
}*/

void Node::print() const {
    int x, y;
    ST::unzipCoordinates(this->getState().getId(), x, y);
    cout << "(" << y + 1 << "," << x + 1 << ")";
}
