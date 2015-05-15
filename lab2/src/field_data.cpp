//
// Created by ivan on 5/7/15.
//

#include <stdio.h>
#include "field_data.h"

void FieldData::print() const {
    char c = '_';
    if (isWumpus) c = 'W'; else if (isPit) c = 'P'; else if (isTeleport) c = 'T';
    printf("%c ", c);
}