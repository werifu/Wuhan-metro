//
// Created by chen on 2/18/21.
//

#include "metro_time.h"
#include "../models/times.h"
#include "../models/trafficFlows.h"

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

double GetPathTime(Path* path, timestamp_t now) {
    double result = 0;
    int edgeNum = path->edgeNum;
    for (int i = 0; i < edgeNum; i++) {
        Edge* e = path->edgesOnPath[i];
        double crowded = GetCrowded(e->metro->trafficFlow, now);
        double coefficient = GetTimeCoefficient(crowded);
        if (coefficient == DBL_MAX) return DBL_MAX;
        double cost = (TWO_STATIONS_RUN_TIME+STATION_STOP_TIME)*coefficient;
        result += cost;
        now += cost;
    }

    int transferNum = GetTransferNum(path);
    result += transferNum * TRANSFER_TIME;
    return result;
}


double GetArrivalTime(Path* path, timestamp_t startTime) {
    double path_time = GetPathTime(path, startTime);
    if (path_time == DBL_MAX) return DBL_MAX;
    return startTime + path_time;
}