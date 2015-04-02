//
// Created by Ivan on 02-Apr-15.
//

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <vector>


#include "map_loader.h"
#include "star_trek_defs.h"


void loadMapFromFile(std::string path, std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& transitions, int &initialId) {
    using namespace std;

    ifstream mapConfigFile;
    mapConfigFile.open(path);

    if (!mapConfigFile.is_open()) {
        cerr << "Unable to open config file." << endl<<path<<endl;
        exit(1);
    }

    transitions.resize((unsigned int) TELEPORT_TRANSITION_OFFSET + 1);

    string line;

    while (getline(mapConfigFile, line)) {

        vector<int> row;

        cout << line << endl;

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
                        transitions[SHUTTLE_TRANSITION_OFFSET].push_back(ST::zipCoordinates(row.size(), map.size()));
                        switch (sub[1]) {
                            case 'S':
                                row.push_back(SHUTTLE_LAUNCH_PAD);
                                break;
                            case 'L':
                                row.push_back(SHUTTLE_LANDING_PAD);
                                break;
                            default:
                                cout << "1" << endl;
                                cerr << "Invalid config file." << endl;
                                exit(1);
                        }
                        break;
                    case 'T': {
                        sub = sub.substr(1);
                        int idx = atoi(sub.c_str());

                        int offset = TELEPORT_TRANSITION_OFFSET + idx;
                        if (transitions.size() <= offset)
                            transitions.resize((unsigned int) (offset + 1));
                        transitions[offset].push_back(ST::zipCoordinates(row.size(), map.size()));

                        row.push_back(TELEPORT_MAP_OFFSET - idx);
                    }
                        break;
                    case 'P':
                        initialId = ST::zipCoordinates(row.size(), map.size());
                        row.push_back(CONTROL_BRIDGE);
                        break;
                    case 'C':
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
}