//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
#define _SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_


#include "uniform_cost_search.h"
#include "star_trek_defs.h"

class ST_uniform_cost_search : public UniformCostSearch {
private:
    std::vector<std::vector<int>> map;

    std::vector<std::vector<int>> transitions;

    int initialId;

    int mapHalfSize;

    virtual std::vector<State> succFunct(State const s) const;

    virtual bool goalFunct(State const s) const;

    inline State toState(int x, int y) const {
        return State(ST::zipCoordinates(x, y));
    }


    virtual int getDistance(State const &a, State const &b) const;

    inline void fromState(State state, int &x, int &y) const {
        ST::unzipCoordinates(state.getId(), x, y);
    };

    inline int getHeight(int x, int y) const {
        int height = map[y][x];
        return height>0?height:0;
    }

    inline bool isShuttle(int x, int y) const {
        return map[y][x] == SHUTTLE_LAUNCH_PAD || map[y][x] == SHUTTLE_LANDING_PAD;
    }
    virtual NodePtr search(State initialState) const override;

public:
    ST_uniform_cost_search(std::string path);

    virtual void run() const;

};


#endif //_SESTI_SEMESTAR_ST_UNIFORM_COST_SEARCH_H_
