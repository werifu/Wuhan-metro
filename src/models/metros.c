//
// Created by chen on 3/1/21.
//
#include "metros.h"
#include "models.h"
#include "common.h"
#include "malloc.h"
#include "../utils/hash.h"
#include "stdlib.h"
#include "trafficFlows.h"

extern MetroSystem* metroSystem;
extern MetroTable metroTable;
extern TrafficFlow* trafficFlowTable[TRAFFIC_FLOW_TYPE_NUM];

int InitMetroSystem() {
    metroSystem = (MetroSystem*)malloc(sizeof(MetroSystem));
    metroSystem->metroNum = 0;
    return TRUE;
}

int InitMetroTable() {
    metroTable = (MetroTable)malloc(sizeof(MetroHash*) * METRO_TABLE_LEN);
    for (int i = 0; i < METRO_TABLE_LEN; i++) {
        metroTable[i] = (MetroHash*)malloc(sizeof(MetroHash));
        metroTable[i]->cur = NULL;
        metroTable[i]->next = NULL;
    }
    return TRUE;
}




int LoadMetros() {
    char metros_filename[] = "../data/metros.csv";
    FILE* fp = fopen(metros_filename, "r");
    if (!fp) {
        return ERROR;
    }
    char line[1024];
    fgets(line, 1024, fp);

    char name[MAX_METRO_NAME_LEN];
    char flowTypeName[20];
    double length;
    int capacity, flowTypeID;
    while (fgets(line, 1024, fp)) {
        const char* tok = strtok(line, ",");
        strcpy(name, tok);

        tok = strtok(NULL,",");
        capacity = atoi(tok);

        tok = strtok(NULL,",");
        length = atof(tok);

        // station nums
        strtok(NULL,",");

        tok = strtok(NULL,"\n");
        strcpy(flowTypeName,tok);
        flowTypeID = ChooseFlowType(flowTypeName);
        Metro* metro = NewMetro(name, length, capacity, trafficFlowTable[flowTypeID]);

        InsertMetro(metroTable, metro);
    }
    return TRUE;
}


Metro* NewMetro(char* name, double length, int capacity, TrafficFlow* trafficFlow) {
    Metro* metro = (Metro*)malloc(sizeof(Metro));
    strcpy(metro->name, name);
    metro->length = length;
    metro->stationNum = 0;
    metro->capacity = capacity;
    metro->trafficFlow = trafficFlow;
    return metro;
}
