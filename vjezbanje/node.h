//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_NODE_H_
#define _SEARCH_NODE_H_

#include "state.h"

#include <memory>
#include <stack>
#include <vector>

class Node;

typedef std::shared_ptr<Node> NodePtr;

class Node {
private:
    State state;
    int current_cost;
    int estimated_cost;
    NodePtr parent;


public:
    Node(State const &state, int current_cost, int estimated_cost, NodePtr parent)
            : state(state), current_cost(current_cost), estimated_cost(estimated_cost), parent(parent) {
    }

    Node(const Node &) = delete;                 // Prevent copy-construction
    Node &operator=(const Node &) = delete;      // Prevent assignment


    bool operator<(Node const &other) const;

    void print() const;

    static void pathReconstruction(NodePtr const &leaf, std::stack<NodePtr> &nodeStack);

    /*    static void pathPrint(stack<NodePtr> &nodeStack) {
        NodePtr node;

        while (!nodeStack.empty()) {
            node = nodeStack.top();
            nodeStack.pop();
            node->print();
        }
    }*/

/*
    class Compare {
    public:
        inline bool operator()(NodePtr a, NodePtr b) {
            return *a < *b;
        }
    };*/

    inline int getCurrentCost() const {
        return current_cost;
    }

    inline NodePtr getParent() const {
        return parent;
    }

    inline State getState() const {
        return state;
    }

    inline int getEstimatedCost() const {
        return estimated_cost;
    }

    void print_w_depth() const;
};


#endif //_SEARCH_NODE_H_
