//
// Created by chen on 2/18/21.
//

#ifndef WUHAN_METRO_METRO_TIME_H
#define WUHAN_METRO_METRO_TIME_H
#include "float.h"
#include "models.h"
#include "paths.h"
#include "common.h"


typedef struct Time Time;
struct Time {
    int hour;
    int minute;
};

double GetPathTime(Path* path, Time* now);

int isDuring(SpecialCrowded* spc, Time* now);
#endif //WUHAN_METRO_METRO_TIME_H
