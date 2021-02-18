//
// Created by chen on 2/18/21.
//

#ifndef WUHAN_METRO_PATHS_H
#define WUHAN_METRO_PATHS_H

#include "models.h"
typedef struct Path Path;
struct Path {
    int stationNum;
    Station* stationsOnPath[MAX_STATION_NUM_LEN];
    int interchangeNum;
    Station* interchangeStations[MAX_STATION_NUM_LEN];
    int length;
    int fare;
};


#endif //WUHAN_METRO_PATHS_H
