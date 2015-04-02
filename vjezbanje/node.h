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
    int cost;
    NodePtr parent;


public:
    Node(State const &state, int cost, NodePtr parent) : state(state), cost(cost), parent(parent) {
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

    inline int getCost() const {
        return cost;
    }

    inline NodePtr getParent() const {
        return parent;
    }

    inline State getState() const {
        return state;
    }
};


#endif //_SEARCH_NODE_H_
