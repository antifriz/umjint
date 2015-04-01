//
// Created by Ivan on 23-Mar-15.
//

#include <iostream>
#include <vector>
#include <queue>
#include <forward_list>
#include <set>
#include <memory>
#include <stack>

using namespace std;

class State;

class Node;

typedef shared_ptr<Node> NodePtr;

class State {
private:
    unsigned int id;
public:
    State(unsigned int id) : id(id) {
    }

    bool operator<(State const &other) const {
        return this->id < other.id;
    }

    inline unsigned int getId() const {
        return id;
    }
};

class Node {
private:
    State state;
    unsigned int depth;
    NodePtr parent;
public:
    Node(State const &state, unsigned int depth, NodePtr parent) : state(state), depth(depth), parent(parent) {
    }

    Node(const Node &) = delete;                 // Prevent copy-construction
    Node &operator=(const Node &) = delete;      // Prevent assignment

    inline State getState() const {
        return state;
    }

    inline unsigned int getDepth() const {
        return depth;
    }

    inline NodePtr getParent() const {
        return parent;
    }

    static NodePtr initial(State state) {
        return make_shared<Node>(state, 0, NodePtr());
    }

    bool operator<(Node const &other) const {
        return this->depth > other.depth;//this->state < other.state;
    }

    void print() const {
        cout << string(this->getDepth(), ' ') << this->getState().getId() << endl;
    }

    static void pathReconstruction(NodePtr const &leaf, stack<NodePtr> &nodeStack) {
        NodePtr node = leaf;

        while (node) {
            nodeStack.push(node);
            node = node->getParent();
        }
    }

    static void pathPrint(stack<NodePtr> &nodeStack) {
        NodePtr node;

        while (!nodeStack.empty()) {
            node = nodeStack.top();
            nodeStack.pop();
            node->print();
        }
    }

    class Compare {
    public:
        bool operator()(NodePtr a, NodePtr b) {
            return *a < *b;
        }
    };
};


forward_list<NodePtr> const expand(NodePtr const parent, vector<State> (*succ)(State const)) {
    vector<State> container = succ(parent->getState());
    forward_list<NodePtr> succQueue;

    for (vector<State>::iterator it = container.begin(); it != container.end(); ++it) {
        succQueue.push_front(make_shared<Node>(*it, parent->getDepth() + 1, parent));
    }

    //cout<<"expand: "<<distance(begin(succQueue),end(succQueue))<<endl;

    return succQueue;
}

NodePtr search(State s0, vector<State> (*succ)(State const), bool (*goal)(State const)) {
    priority_queue<NodePtr, vector<NodePtr>, Node::Compare> openQueue;
    openQueue.push(Node::initial(s0));

    set<State> discoveredSet;

    discoveredSet.insert(s0);

    while (!openQueue.empty()) {
        NodePtr n = openQueue.top();
        openQueue.pop();

        //cout<<"size: "<<openQueue.size()<<endl;

        if (goal(n->getState())) return n;

        forward_list<NodePtr> container = expand(n, succ);
        for (forward_list<NodePtr>::iterator it = container.begin(); it != container.end(); ++it) {
            Node &item = **it;
            if (discoveredSet.find(item.getState()) == discoveredSet.end()) {
                discoveredSet.insert(item.getState());
                //cout<<"pushed "<<it->getState().getId() <<endl;
                openQueue.push(*it);
            }
        }
    }
    return nullptr;
}


vector<State> succFunct(State const s) {
    vector<State> container;

    container.push_back(State(s.getId() * 2));

    container.push_back(State(s.getId() * 2 + 1));
    return container;
}

bool goalFunct(State const s) {
    return s.getId() > 63;
}

#include <fstream>

int main() {

    ifstream mapConfigFile;
    mapConfigFile.open("konfiguracija.txt");

    if(!mapConfigFile.is_open())
    {
        cerr<<"Unable to open config file."<<endl;
        return 1;
    }

    string line;

    while(getline(mapConfigFile,line))
    {
        cout<<line<<endl;
    }
    mapConfigFile.close();




    cout << "start" << endl;

    NodePtr node;

    node = search(State(1), succFunct, goalFunct);

    stack<NodePtr> nodeStack;

    Node::pathReconstruction(node, nodeStack);

    Node::pathPrint(nodeStack);

    cout << "fin" << endl;
    return 0;
}



