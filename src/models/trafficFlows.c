//
// Created by chen on 3/1/21.
//

#include "trafficFlows.h"
#include "malloc.h"
#include "common.h"
#include "string.h"
#include "times.h"
extern TrafficFlow* trafficFlowTable[TRAFFIC_FLOW_TYPE_NUM];


int InitTrafficFlowTable() {
    // type0: work
    trafficFlowTable[0] = (TrafficFlow*)malloc(sizeof(TrafficFlow));
    trafficFlowTable[0]->defaultCrowded = 0.4;
    AddSpecialCrowd(0, 7, 30, 9, 0, 0.8);
    AddSpecialCrowd(0, 16, 30, 18, 30, 0.75);

    // type1: shopping
    trafficFlowTable[1] = (TrafficFlow*)malloc(sizeof(TrafficFlow));
    trafficFlowTable[1]->defaultCrowded = 0.2;
    AddSpecialCrowd(1, 9, 30, 15, 0, 0.65);

    // type2: entertainment
    trafficFlowTable[2] = (TrafficFlow*)malloc(sizeof(TrafficFlow));
    trafficFlowTable[2]->defaultCrowded = 0.15;
    AddSpecialCrowd(2, 19, 0, 22, 0, 0.65);

    // type3: cities
    trafficFlowTable[3] = (TrafficFlow*)malloc(sizeof(TrafficFlow));
    trafficFlowTable[3]->defaultCrowded = 0.5;

    return TRUE;
}


int ChooseFlowType(const char* type) {
    if (strcmp(type, "work") == 0) {
        return 0;
    } else if (strcmp(type, "shopping") == 0) {
        return 1;
    } else if (strcmp(type, "entertainment") == 0) {
        return 2;
    } else {
        return 3; // default: cities
    }
}


int AddSpecialCrowd(int typeID, int startHour, int startMin, int endHour, int endMin, double crowded) {
    SpecialCrowded* newCrowded = (SpecialCrowded*)malloc(sizeof(SpecialCrowded));
    newCrowded->startStp = Clock2Stamp(startHour, startMin);
    newCrowded->endStp = Clock2Stamp(endHour, endMin);
    newCrowded->crowded = crowded;
    int specialNum = trafficFlowTable[typeID]->specialNum;
    trafficFlowTable[typeID]->specialCrowdeds[specialNum] = newCrowded;
    trafficFlowTable[typeID]->specialNum++;
    return TRUE;
}


int SetDefaultCrowd(TrafficFlow* trafficFlow, double new_crowd) {
    if (!trafficFlow) return ERROR;
    trafficFlow->defaultCrowded = new_crowd;
    return TRUE;
}


double GetCrowded(TrafficFlow* trafficFlow, timestamp_t now) {
    if (trafficFlow->defaultOnly) return trafficFlow->defaultCrowded;
    int n = trafficFlow->specialNum;
    for (int i = 0; i < n; i++) {
        SpecialCrowded* spc = trafficFlow->specialCrowdeds[i];
        if (IsDuring(spc, now)) {
            return spc->crowded;
        }
    }
    return trafficFlow->defaultCrowded;
}


TrafficFlow* DeepCopyTrafficFlow(TrafficFlow* tobeCopied) {
    TrafficFlow* result = (TrafficFlow*)malloc(sizeof(TrafficFlow));
    result->defaultCrowded = tobeCopied->defaultCrowded;
    result->defaultOnly = FALSE;
    memcpy(result->specialCrowdeds,tobeCopied->specialCrowdeds, sizeof(tobeCopied->specialCrowdeds));
    result->specialNum = tobeCopied->specialNum;
    return result;
}

int UseOnlyDefaultCrowd(Metro* metro) {
    metro->trafficFlow->defaultOnly = TRUE;
    return TRUE;
}
