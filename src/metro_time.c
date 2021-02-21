//
// Created by chen on 2/18/21.
//

#include "metro_time.h"


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

double GetCrowded(TrafficFlow* trafficFlow, Time* now) {
    int n = trafficFlow->specialNum;
    for (int i = 0; i < n; i++) {
        SpecialCrowded* spc = trafficFlow->specialCrowdeds[i];
        if (isDuring(spc, now)) {
            return spc->crowded;
        }
    }
    return trafficFlow->defaultCrowded;
}

int isDuring(SpecialCrowded* spc, Time* now) {
    if (now->hour >= spc->startHour && now->minute >= spc->startMinute && now->hour <= spc->endHour && now->minute < spc->endMinute) {
        return TRUE;
    }
    return FALSE;
}


double GetPathTime(Path* path, Time* now) {
    double result = 0;
    int edgeNum = path->edgeNum;
    for (int i = 0; i < edgeNum; i++) {
        Edge* e = path->edgesOnPath[i];
        double crowded = GetCrowded(e->metro->trafficFlow, now);
        double coefficient = GetTimeCoefficient(crowded);
        result += (TWO_STATIONS_RUN_TIME+STATION_STOP_TIME)*coefficient;
    }
    int transferNum = path->transferNum;
    result += transferNum * TRANSFER_TIME;
    return result;
}

