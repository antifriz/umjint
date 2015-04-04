//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_STAR_TREK_H_
#define _SEARCH_STAR_TREK_H_

#define CONTROL_BRIDGE -1
#define MEETING_PLACE -2
#define SHUTTLE_LAUNCH_PAD -3
#define SHUTTLE_LANDING_PAD -4
#define TELEPORT_MAP_OFFSET -5

#define SHUTTLE_TRANSITION_OFFSET 0
#define TELEPORT_TRANSITION_OFFSET 1

class ST {
public:
    inline static int zipCoordinates(int x, int y) {
        return (x << 16) + y;
    }

    inline static int unzipY(int z) {
        return z & 0xFF;
    }
    inline static int unzipX(int z) {
        return z >> 16;
    }

    inline static void unzipCoordinates(int z, int& x, int& y) {
        x = unzipX(z);
        y = unzipY(z);
    }
};

#endif //_SEARCH_STAR_TREK_H_
