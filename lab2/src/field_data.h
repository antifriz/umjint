//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_FIELD_DATA_H
#define PROJECT_FIELD_DATA_H

#include "atom.h"

class FieldData {
public:
    FieldData(bool isStench, bool isBreeze, bool isGlow, bool isWumpus, bool isPit, bool isTeleport) : isStench(
            isStench), isBreeze(isBreeze), isGlow(isGlow), isWumpus(isWumpus), isPit(isPit), isTeleport(isTeleport) { }

    template<Property property>
    bool is() const {
        switch (property) {
            case Property::Stench:
                return isStench;
            case Property::Breeze:
                return isBreeze;
            case Property::Glow:
                return isGlow;
            case Property::Wumpus:
                return isWumpus;
            case Property::Pit:
                return isPit;
            case Property::Teleport:
                return isTeleport;
            default:
                throw "Invalid property";
        }
    }

    void print() const;

private:
    bool isStench;
    bool isBreeze;
    bool isGlow;
    bool isWumpus;
    bool isPit;
    bool isTeleport;

};


#endif //PROJECT_FIELD_DATA_H
