//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_STATE_H_
#define _SEARCH_STATE_H_


class State {
private:
    int id;
public:
    State(int id) : id(id) {
    }

    inline bool operator<(State const &other) const {
        return this->id < other.id;
    }

    inline int getId() const {
        return id;
    }
};


#endif //_SEARCH_STATE_H_
