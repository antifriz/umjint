//
// Created by ivan on 5/6/15.
//

#ifndef PROJECT_REFUTATION_RESOLUTION_H
#define PROJECT_REFUTATION_RESOLUTION_H


#include "lazy_defines.h"
#include "atom.h"
#include <queue>
#include <assert.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "knowledge_base.h"

class KnowledgeBase;

class RefutationResolution {
public:
    static bool deduce(const KnowledgeBase &kb, const Clause &negatedConsequence);
};


#endif //PROJECT_REFUTATION_RESOLUTION_H
