//
// Created by chen on 2/18/21.
//

#ifndef WUHAN_METRO_PATHS_H
#define WUHAN_METRO_PATHS_H

#include "handler_models.h"

Path* GetShortestPath(Station* src,Station* dest, timestamp_t startTime);

Path* GetLeastTimePath(Station* src, Station* dest, timestamp_t startTime);

Path* GetAvoidingCrowdPath(Station* src, Station* dest, timestamp_t startTime);

Station* UpdateAdjStatusUponLen(Station* station, Metro* cur_metro);

Station* UpdateAdjStatusUponTime(Station* station, Metro* cur_metro);

int GetTransferNum(Path* path);

int GetPathEdgeNumFromDest(Station* station);

int SetPathFromDest(Path* result, Station* dest);

Edge* GetEdgeBetweenAdj(Station* station1, Station* station2);

int CanReach(Station* dest);
#endif //WUHAN_METRO_PATHS_H
