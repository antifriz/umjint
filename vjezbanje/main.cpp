//
// Created by Ivan on 23-Mar-15.
//
#include "ST_uniform_cost_search.h"
#include <iostream>


using namespace std;


int main() {


    NodePtr node;

    ST_uniform_cost_search stucs("konfiguracija.txt");
    stucs.run();


    cout << "fin" << endl;
    return 0;
}