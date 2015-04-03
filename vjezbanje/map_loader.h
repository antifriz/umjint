//
// Created by Ivan on 02-Apr-15.
//

#ifndef _SEARCH_MAP_LOADER_H_
#define _SEARCH_MAP_LOADER_H_

#include <string>
#include <vector>
#include "state.h"

void loadMapFromFile(std::string path, std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& vector, int& initialId, State& goalStatex);

#endif //_SEARCH_MAP_LOADER_H_
