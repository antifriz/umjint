//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
#define _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_


#include "general_search_algorithm.h"
#include "star_trek_defs.h"

class ST_uniform_cost_search : public GeneralSearchAlgorithm {
private:
    std::vector<std::vector<int>> map;

    std::vector<std::vector<int>> transitions;

    int initialId;

    State goalState = State(0);

    int mapHalfSize;


protected:
    virtual int distanceFunc(State const &a, State const &b) const;

    virtual int heuristicFunc(State const &s) const;

    virtual const std::vector<State> succFunc(State const &state) const;

    virtual bool goalFunc(State const &state) const;


    static inline State toState(int x, int y) {
        return State(ST::zipCoordinates(x, y));
    }

    static inline void fromState(State state, int &x, int &y) {
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
    ST_uniform_cost_search(std::string path);

    void run();

};


#endif //_SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
