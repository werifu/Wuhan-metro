//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_HANDLER_MODELS_H
#define WUHAN_METRO_HANDLER_MODELS_H
#include "../models/common.h"
#include "../models/models.h"

typedef struct Time Time;
struct Time {
    int hour;
    int minute;
};

typedef struct Path Path;
struct Path {
    int edgeNum;
    Edge* edgesOnPath[MAX_STATION_NUM_LEN];
    int transferNum;
    Station* transferStations[MAX_STATION_NUM_LEN];
    int length;
    int fare;
};

#endif //WUHAN_METRO_HANDLER_MODELS_H
