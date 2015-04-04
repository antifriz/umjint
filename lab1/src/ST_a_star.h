//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
#define _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_


#include "a_star.h"
#include "star_trek_defs.h"
#include <map>

class ST_a_star : public GeneralSearchAlgorithm {
private:
    std::vector<std::vector<int>> map;

    std::vector<std::vector<int>> transitions;

    int initialId;

    State goalState = State(0);

    int mapHalfSize;

    std::map<int, int> adrelTransporters;
    std::map<int, int> enterpriseTransporters;


    void loadMapFromFile(std::string path);

protected:
    int distanceFunc(State const& a, State const& b) const;

    int heuristicFunc(State const& s) const;

    const std::vector<State> succFunc(State const& state) const;

    bool goalFunc(State const& state) const;


    static inline State toState(int x, int y) {
        return State(ST::zipCoordinates(x, y));
    }

    static inline void fromState(State state, int& x, int& y) {
        ST::unzipCoordinates(state.getId(), x, y);
    };

    inline int getHeight(int x, int y) const {
        int height = map[y][x];
        return height > 0 ? height : 0;
    }

    inline bool isShuttle(int x, int y) const {
        return map[y][x] == SHUTTLE_LAUNCH_PAD || map[y][x] == SHUTTLE_LANDING_PAD;
    }

public:
    ST_a_star(std::string path);

    void run();

    inline int getManhattanDistance(int xa, int ya, int xb, int yb) const {
        return abs(getHeight(xa, ya) - getHeight(xb, yb));

    }

    inline int getManhattanDistance(int ida, int idb) const {
        int xa, ya, xb, yb;
        ST::unzipCoordinates(ida, xa, ya);
        ST::unzipCoordinates(idb, xb, yb);
        return getManhattanDistance(xa, ya, xb, yb);
    }

    int getManhattanDistanceOverTransporter(int fromId, bool adrelOnes)const;

    void setupManhattanDistanceOverTransporters();
};


#endif //_SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
