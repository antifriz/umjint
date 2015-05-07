//
// Created by ivan on 5/7/15.
//

#include <stdio.h>
#include "field_data.h"

void FieldData::print() const {
    char c = '_';
    int i = 0;
    if (isStench) i += 0x1;
    if (isBreeze) i += 0x10;
    if (isGlow) i += 0x100;
    if (isWumpus) c = 'W'; else if (isPit) c = 'P'; else if (isTeleport) c = 'T';
    printf("%c ", c);
}