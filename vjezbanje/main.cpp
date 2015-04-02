//
// Created by Ivan on 23-Mar-15.
//
#include "map_loader.h"
#include "ST_uniform_cost_search.h"
#include <iostream>


using namespace std;


int main() {
    vector<vector<int>> map;

    loadMapFromFile("konfiguracija.txt", map);

    int mapHalfSize = map.size() >> 1;

    cout << "start" << endl;

    NodePtr node;

    ST_uniform_cost_search stucs;

    node = stucs.search(State(1));

    stack<NodePtr> nodeStack;

    Node::pathReconstruction(node, nodeStack);

    //Node::pathPrint(nodeStack);

    cout << "fin" << endl;
    return 0;
}