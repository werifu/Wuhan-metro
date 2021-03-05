//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_TRAVERSE_H
#define WUHAN_METRO_TRAVERSE_H
#include "../models/models.h"
int TraverseSystem();

void DFS_System(Station* station);

int TraverseMetroViaEdge(char* name);

void PrintGraph();

void PrintAdjVertex();

void PrintFromDest(Station* station);

void PrintCurStationArrivalTime(Station* station);
#endif //WUHAN_METRO_TRAVERSE_H
