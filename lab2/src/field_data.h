//
// Created by ivan on 5/7/15.
//

#ifndef PROJECT_FIELD_DATA_H
#define PROJECT_FIELD_DATA_H

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
                return isStench;
            default:
                throw "Invalid property";
        }
    }

    void print() const {
        char c = '_';
        int i = 0;
        if (isStench) i += 0x1;
        if (isBreeze) i += 0x10;
        if (isGlow) i += 0x100;
        if (isWumpus) c = 'W'; else if (isPit) c = 'P'; else if (isTeleport) c = 'T';
        printf("%c ", c);
    }

private:
    bool isStench;
    bool isBreeze;
    bool isGlow;
    bool isWumpus;
    bool isPit;
    bool isTeleport;

};

#include <string>

#endif //PROJECT_FIELD_DATA_H
