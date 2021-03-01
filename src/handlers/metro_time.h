//
// Created by chen on 2/18/21.
//

#ifndef WUHAN_METRO_METRO_TIME_H
#define WUHAN_METRO_METRO_TIME_H
#include "float.h"
#include "../models/models.h"
#include "paths.h"
#include "../models/common.h"
#include "handler_models.h"

// Consists of station2station_time, station_waiting_time, crowd_level
double GetPathTime(Path* path, Time* now);

int isDuring(SpecialCrowded* spc, Time* now);
#endif //WUHAN_METRO_METRO_TIME_H
