//
// Created by Ivan on 23-Mar-15.
//
#include <iostream>

#include "ST_a_star.h"
using namespace std;


void runSearch(string s){
    cout<<s<<endl;
    ST_a_star star(s);
    star.run();
    cout<<endl;
}

int main() {

    NodePtr node;

    runSearch("res/konfiguracija1.txt");
    runSearch("res/konfiguracija2.txt");
    runSearch("res/konfiguracija3.txt");

    //cout << "fin" << endl;
    return 0;
}