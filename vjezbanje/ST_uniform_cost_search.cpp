//
// Created by Ivan on 02-Apr-15.
//

#include "ST_uniform_cost_search.h"
#include "map_loader.h"
#include <algorithm>
#include <iostream>
#include <conio.h>

using namespace std;


ST_uniform_cost_search::ST_uniform_cost_search(std::string path) {
    loadMapFromFile(path, map, transitions, initialId);

    mapHalfSize = map.size() >> 1;

    cout << "map half size: " << mapHalfSize << endl;
}

void ST_uniform_cost_search::run() {
    cout << (search(initialId) ? "OK" : "NOT OK") << endl;
}

int ST_uniform_cost_search::distanceFunc(State const &a, State const &b) const {
    int xa, ya, xb, yb;
    fromState(a, xa, ya);
    fromState(b, xb, yb);
    int manhattan_dist = abs(xb - xa) + abs(yb - ya);

    cout << "height (" << xa << "," << ya << ") - (" << xb << "," << yb << "): " << getHeight(xa, ya) << " " << getHeight(xb, yb) << " " << manhattan_dist << endl;

    if (manhattan_dist == 1) return abs(getHeight(xa, ya) - getHeight(xb, yb));
    if (isShuttle(xa, xa)) manhattan_dist *= 3;
    return manhattan_dist;
}

int ST_uniform_cost_search::heuristicFunc(State const &s) const {
    return 0;
}

const std::vector<State> ST_uniform_cost_search::succFunc(State const &state) const {
    int x0, y0;
    fromState(state, x0, y0);

    vector<State> container;

    // left
    if (x0 != 0 && x0 != mapHalfSize) {
        //cout << "left" << endl;
        container.push_back(toState(x0 - 1, y0));
    }

    // right
    if (x0 != (mapHalfSize - 1) && x0 != ((mapHalfSize << 1) - 1)) {
        //cout << "right" << endl;
        container.push_back(toState(x0 + 1, y0));
    }

    // up
    if (y0 != 0) {
        //cout << "up" << endl;
        container.push_back(toState(x0, y0 - 1));
    }

    // down
    if (y0 != ((mapHalfSize << 1) - 1)) {
        //cout << "down" << endl;
        container.push_back(toState(x0, y0 + 1));
    }

    // shuttles 'n' teleports

    int iter = 0;
    for (auto v: transitions) {
/*        cout << iter++ << " -> ";
        for (auto i: v) {
            int x, y;
            ST::unzipCoordinates(i, x, y);
            cout << "(" << x << "," << y << "), ";
        }
        cout << endl;*/
        if (find(v.begin(), v.end(), state.getId()) != v.end()) {
            for (auto i: v)
                if (i != state.getId()) {
                    int x, y;
                    ST::unzipCoordinates(i, x, y);
                    cout << "dodan skok: (" << x << "," << y << ")" << endl;
                    container.push_back(State(i));
                }
            cout << "shuttle/teleports" << endl;
            break;
        }
    }

    cout << container.size() << endl;

    cout << "press key" << endl;
    getch();
    getch();
    return container;
}

bool ST_uniform_cost_search::goalFunc(State const &state) const {
    int x, y;
    fromState(state, x, y);
    return map[y][x] == MEETING_PLACE;
}
