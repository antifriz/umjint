//
// Created by Ivan on 02-Apr-15.
//

#include "ST_a_star.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;


ST_a_star::ST_a_star(std::string path) {
    loadMapFromFile(path);

    setupManhattanDistanceOverTransporters();
}

void ST_a_star::run() {

    NodePtr n = search(initialId);

    cout << "Minimal cost: " << n->getCurrentCost() << endl;
    cout << "Opened nodes: " << getOpenNodeCount() << endl;


    //cout << (n ? "OK" : "NOT OK") << endl;


    stack<NodePtr> stack;
    Node::pathReconstruction(n, stack);

    while (!stack.empty()) {
        stack.top()->print();
        stack.pop();
        if (!stack.empty())
            cout << " ->";
        cout << endl;
    }
    //cout << this->getOpenNodeCount() << endl;
}

int ST_a_star::distanceFunc(State const& a, State const& b) const {
    int xa, ya, xb, yb;
    fromState(a, xa, ya);
    fromState(b, xb, yb);
    int manhattan_dist = abs(xb - xa) + abs(yb - ya);

    //cout << "height (" << xa << "," << ya << ") - (" << xb << "," << yb << "): " << getHeight(xa, ya) << " " << getHeight(xb, yb) << " " << manhattan_dist << endl;

    if (manhattan_dist == 1) return getManhattanDistance(xa, ya, xb, yb);
    if (isShuttle(xa, ya)) manhattan_dist *= 3;
    return manhattan_dist;
}

inline int ST_a_star::heuristicFunc(State const& s) const {
#if defined(HEUR1)
    cout<<"2a"<<endl; 
        return distanceFunc(s, goalState);
#elif defined(HEUR2)

    cout<<"2b"<<endl;
    return getManhattanDistanceOverTransporter(s.getId(), ST::unzipX(s.getId()) >= mapHalfSize);
#else

    cout<<"1"<<endl;
    return 0;
#endif
}

const std::vector<State> ST_a_star::succFunc(State const& state) const {
    int x0, y0;
    fromState(state, x0, y0);

    vector<State> container;

    // shuttles 'n' teleports

    //int iter = 0;
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
                    //cout << "dodan skok: (" << x << "," << y << ")" << endl;
                    container.push_back(State(i));
                }
            //cout << "shuttle/teleports" << endl;
            break;
        }
    }



    // right
    if (x0 != (mapHalfSize - 1) && x0 != ((mapHalfSize << 1) - 1)) {
        //cout << "right" << endl;
        container.push_back(toState(x0 + 1, y0));
    }

    // down
    if (y0 != ((mapHalfSize << 1) - 1)) {
        //cout << "down" << endl;
        container.push_back(toState(x0, y0 + 1));
    }

    // up
    if (y0 != 0) {
        //cout << "up" << endl;
        container.push_back(toState(x0, y0 - 1));
    }

    // left
    if (x0 != 0 && x0 != mapHalfSize) {
        //cout << "left" << endl;
        container.push_back(toState(x0 - 1, y0));
    }


    //cout << container.size() << endl;

    //cout << "press key" << endl;
    //getchar();
    return container;
}

bool ST_a_star::goalFunc(State const& state) const {
    int x, y;
    fromState(state, x, y);
    return map[y][x] == MEETING_PLACE;
}

void ST_a_star::loadMapFromFile(std::string path) {
    using namespace std;

    ifstream mapConfigFile;
    mapConfigFile.open(path);

    if (!mapConfigFile.is_open()) {
        cerr << "Unable to open config file." << endl << path << endl;
        exit(1);
    }

    transitions.resize((unsigned int) TELEPORT_TRANSITION_OFFSET + 1);

    string line;

    while (getline(mapConfigFile, line)) {

        vector<int> row;

        //cout << line << endl;

        istringstream iss(line);
        do {
            string sub;
            iss >> sub;

            if (sub.length() == 0) continue;

            if (isdigit(sub[0]))
                row.push_back(atoi(sub.c_str()));
            else {
                switch (sub[0]) {
                    case 'S':
                        transitions[SHUTTLE_TRANSITION_OFFSET].push_back(ST::zipCoordinates((int) row.size(), (int) map.size()));
                        switch (sub[1]) {
                            case 'S':
                                row.push_back(SHUTTLE_LAUNCH_PAD);
                                break;
                            case 'L':
                                row.push_back(SHUTTLE_LANDING_PAD);
                                break;
                            default:
                                cerr << "Invalid config file." << endl;
                                exit(1);
                        }
                        break;
                    case 'T': {
                        sub = sub.substr(1);
                        int idx = atoi(sub.c_str());

                        int offset = TELEPORT_TRANSITION_OFFSET + idx;
                        if ((int)transitions.size() <= offset)
                            transitions.resize((unsigned int) (offset + 1));
                        transitions[offset].push_back(ST::zipCoordinates((int) row.size(), (int) map.size()));

                        row.push_back(TELEPORT_MAP_OFFSET - idx);
                    }
                        break;
                    case 'P':
                        initialId = ST::zipCoordinates((int) row.size(), (int) map.size());
                        row.push_back(CONTROL_BRIDGE);
                        break;
                    case 'C':
                        goalState = State(ST::zipCoordinates((int) row.size(), (int) map.size()));
                        row.push_back(MEETING_PLACE);
                        break;
                    default:
                        cerr << "Invalid config file." << endl;
                        exit(1);
                }
            }
        } while (iss);

        map.push_back(row);

        if (row.size() != map[0].size()) {
            cerr << "Invalid config file." << endl;
            exit(1);
        }
    }
    mapConfigFile.close();

    if (map.size() != map[0].size() || map.size() & 0x1) {
        cerr << "Invalid config file." << endl;
        exit(1);
    }

    mapHalfSize = (int) (map.size() >> 1);


}

void ST_a_star::setupManhattanDistanceOverTransporters() {
    int i = 0;
    for (auto& transition: transitions)
        if (i++ >= TELEPORT_TRANSITION_OFFSET)
            for (auto& transporter: transition)
                if (mapHalfSize > ST::unzipY(transporter))
                    adrelTransporters.insert(make_pair(transporter, getManhattanDistance(transporter, goalState.getId())));
                else
                    enterpriseTransporters.insert(make_pair(transporter, 0));

    for (auto& transporter: enterpriseTransporters)
        transporter.second=getManhattanDistanceOverTransporter(transporter.first, true);
}

int ST_a_star::getManhattanDistanceOverTransporter(int fromId, bool adrelOnes) const {
    int ultMinDist = getManhattanDistance(fromId, goalState.getId());


    int fromDist = INT32_MAX;
    for (auto& transporter: adrelOnes?adrelTransporters:enterpriseTransporters) {
        int adrelDist = fromDist;
        int interDist = getManhattanDistance(fromId, transporter.first);
        int dist = interDist + adrelDist;
        if (dist < fromDist) {
            fromDist = dist;
            assert(fromDist >= ultMinDist);
            if (fromDist == ultMinDist)
                break;
        }
    }
    return fromDist;
}
