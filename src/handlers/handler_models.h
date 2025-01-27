//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_HANDLER_MODELS_H
#define WUHAN_METRO_HANDLER_MODELS_H
#include "../models/common.h"
#include "../models/models.h"


typedef struct Path Path;
struct Path {
    int     edgeNum;
    Edge*   edgesOnPath[MAX_STATION_NUM_LEN];
    double  length;
};

#endif //WUHAN_METRO_HANDLER_MODELS_H
