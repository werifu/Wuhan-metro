//
// Created by chen on 2/18/21.
//

#include "metro_time.h"
#include "../models/times.h"


double GetTimeCoefficient(double crowded) {
    if (crowded < 0.2) {
        return 0.6;
    } else if (crowded < 0.5) {
        return 0.8;
    } else if (crowded < 0.75) {
        return 1;
    } else if (crowded < 1) {
        return 1.5;
    } else {
        return DBL_MAX;
    }
}

double GetCrowded(TrafficFlow* trafficFlow, time_t now) {
    int n = trafficFlow->specialNum;
    for (int i = 0; i < n; i++) {
        SpecialCrowded* spc = trafficFlow->specialCrowdeds[i];
        if (IsDuring(spc, now)) {
            return spc->crowded;
        }
    }
    return trafficFlow->defaultCrowded;
}


double GetPathTime(Path* path, time_t now) {
    double result = 0;
    int edgeNum = path->edgeNum;
    for (int i = 0; i < edgeNum; i++) {
        Edge* e = path->edgesOnPath[i];
        double crowded = GetCrowded(e->metro->trafficFlow, now);
        double coefficient = GetTimeCoefficient(crowded);
        result += (TWO_STATIONS_RUN_TIME+STATION_STOP_TIME)*coefficient;
    }

    int transferNum = GetTransferNum(path);
    result += transferNum * TRANSFER_TIME;
    return result;
}
