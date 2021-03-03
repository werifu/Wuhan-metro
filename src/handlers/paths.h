//
// Created by chen on 2/18/21.
//

#ifndef WUHAN_METRO_PATHS_H
#define WUHAN_METRO_PATHS_H

#include "handler_models.h"

Path* GetShortestPath(Station* src,Station* dest);

Station* UpdateAdjStatus(Station* station);

int GetTransferNum(Path* path);

int GetPathEdgeNumFromDest(Station* station);

int SetPathFromDest(Path* result, Station* dest);

Edge* GetEdgeBetweenAdj(Station* station1, Station* station2);
#endif //WUHAN_METRO_PATHS_H
