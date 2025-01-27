//
// Created by chen on 2/20/21.
//

#include "test.h"
#include "stdio.h"
#include "../models/common.h"
#include "../models/models.h"
#include "../utils/hash.h"
#include "../models/times.h"
extern MetroSystem* metroSystem;
extern StationTable stationTable;
extern MetroTable metroTable;

int TestGetStation() {
    char name[] = "宏图大道";
    Station* station1 = GetStation(name);
    if (!station1) {
        printf("station %s not exists\n", name);
        return FALSE;
    }
    int metroNum = station1->metroNum;
    for (int i = 0; i < metroNum; i++) {
        MetroContext* ctx = station1->metroContexts[i];
        printf("station %s on %s.\t",
               station1->name, ctx->metro->name);
        if (ctx->lastStation) {
            printf("Last station is: %s.\t", ctx->lastStation->name);
        } else {
            printf("No last station.\t");
        }
        if (ctx->nextStation){
            printf("Next station is: %s.\t", ctx->nextStation->name);
        } else {
            printf("No next station.\t");
        }
        if (ctx->lastEdge) {
            printf("Last edge start: %s.\t", ctx->lastEdge->startStation->name);
        } else {
            printf("No last edge.\t");
        }
        if (ctx->nextEdge) {
            printf("Next edge end: %s.\n", ctx->nextEdge->endStation->name);
        } else {
            printf("No next edge.\n");
        }
    }
    return TRUE;
}

int TestFlowType(char* metroName) {
    Metro* metro = GetMetro(metroName);
    if (!metro) {
        printf("无此地铁:%s\n", metroName);
        return FALSE;
    }
    TrafficFlow* flow = metro->trafficFlow;
    printf("metro %s's default crowded: %f\n", metro->name, flow->defaultCrowded);
    for (int i = 0; i < flow->specialNum; i++) {
        SpecialCrowded* crd = flow->specialCrowdeds[i];
        Time startTime;
        Stamp2Time(crd->startStp, &startTime);
        Time endTime;
        Stamp2Time(crd->endStp, &endTime);
        printf("(%2d:%02d,%2d:%02d)=>%f\n", startTime.hour, startTime.minute, endTime.hour, endTime.minute, crd->crowded);
    }
    return TRUE;
}

