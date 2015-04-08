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

int main(int argc,char * argv[]) {
    NodePtr node;

    runSearch("res/konfiguracija1.txt");
    runSearch("res/konfiguracija2.txt");
    runSearch("res/konfiguracija3.txt");
    runSearch("res/konfiguracija4.txt");
    runSearch("res/konfiguracija5.txt");
    runSearch("res/konfiguracija6.txt");
    runSearch("res/konfiguracija7.txt");
    runSearch("res/konfiguracija8.txt");

    return 0;
}