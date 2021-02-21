//
// Created by chen on 2/16/21.
//

#include "models.h"
#include "malloc.h"
#include "common.h"
#include "hash.h"
#include "string.h"
#include "stdlib.h"
MetroSystem* metroSystem;
StationTable stationTable;
MetroTable metroTable;
TrafficFlow* trafficFlowTable[TRAFFIC_FLOW_TYPE_NUM];

int InitMetroSystem() {
    metroSystem = (MetroSystem*)malloc(sizeof(MetroSystem));
    metroSystem->metroNum = 0;
    return TRUE;
}

int InitStationTable() {
    stationTable = (StationTable)malloc(sizeof(StationHash*) * STATION_TABLE_LEN);
    for (int i = 0; i < STATION_TABLE_LEN; i++) {
        stationTable[i] = (StationHash*)malloc(sizeof(StationHash));
        if (stationTable[i] == NULL) printf("cannot allocate StationHash\n");
        stationTable[i]->cur = NULL;
        stationTable[i]->next = NULL;
    }

    for (int i = 0; i < STATION_TABLE_LEN;i++) {
        if (stationTable[i] == NULL) {
            printf("stationTable[%d] is NULL", i);
        }
    }
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
    newCrowded->startHour = startHour;
    newCrowded->endHour = endHour;
    newCrowded->startMinute = startMin;
    newCrowded->endMinute = endMin;
    newCrowded->crowded = crowded;
    int specialNum = trafficFlowTable[typeID]->specialNum;
    trafficFlowTable[typeID]->specialCrowdeds[specialNum] = newCrowded;
    trafficFlowTable[typeID]->specialNum++;
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


Station* NewStation(char* name, Metro* metro, double longitude, double latitude, int id) {
    Station* station = GetStation(stationTable, name);
    if (!station) {
        station = (Station *) malloc(sizeof(Station));
        station->metroNum = 0;
        strcpy(station->name, name);
        station->longitude = longitude;
        station->latitude = latitude;
    }
    MetroContext* context = NewMetroContext(metro, id);
    station->metroContexts[station->metroNum] = context;
    station->metroNum++;

    return station;
}


MetroContext* NewMetroContext(Metro* metro, int id) {
    MetroContext* newContext = (MetroContext*)malloc(sizeof(MetroContext));
    newContext->id = id;
    newContext->metro = metro;
    return newContext;
}


Edge* NewEdge(Station* startStation, Station* endStation, Metro* metro, double length, double timeNeeded) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->startStation = startStation;
    edge->endStation = endStation;
    edge->metro = metro;
    edge->length = length;
    edge->timeNeeded = timeNeeded;
    return edge;
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


int LoadStations() {
    char metros_filename[] = "../data/stations.csv";
    FILE* fp = fopen(metros_filename, "r");
    if (!fp) {
        return ERROR;
    }
    char line[1024];
    fgets(line, 1024, fp);

    char name[MAX_STATION_NAME_LEN];
    int id;
    double longitude, latitude;
    char metro_name[MAX_METRO_NAME_LEN];
    while (fgets(line, 1024, fp)) {
        const char* tok = strtok(line, ",");
        strcpy(name, tok);

        tok = strtok(NULL,",");
        longitude = atof(tok);

        tok = strtok(NULL,",");

        latitude = atof(tok);

        tok = strtok(NULL, ",");
        strcpy(metro_name, tok);
        Metro* metro = GetMetro(metroTable, metro_name);
        if (!metro) {
            fprintf(stderr, "Metro %s Not Found\n", metro_name);
            continue;
        }

        tok = strtok(NULL, ",");
        id = atoi(tok);

        Station* station = NewStation(name, metro, longitude, latitude, id);
        if (!GetStation(stationTable, station->name)) {
            InsertStation(stationTable, station);
        }
        metro->stationNum++;
        metro->stations[id] = station;
    }
    return TRUE;
}


int LoadEdgesAndContexts() {
    char metros_filename[] = "../data/metros.csv";
    FILE* fp = fopen(metros_filename, "r");
    if (!fp) {
        return ERROR;
    }
    char line[1024];
    fgets(line, 1024, fp);

    char name[MAX_METRO_NAME_LEN];
    while (fgets(line, 1024, fp)) { // for each metro
        const char* tok = strtok(line, ",");
        strcpy(name, tok);

        Metro* metro = GetMetro(metroTable, name);
        double all_length = metro->length;
        int num = metro->stationNum;
        if (num == 0) {
            fprintf(stderr, "Metro %s has 0 station\n", metro->name);
            continue;
        }
        double length = all_length/num;
        for (int i = 2; i <= num; i++) {
            Edge* edge = NewEdge(metro->stations[i-1], metro->stations[i], metro, length, TWO_STATIONS_RUN_TIME);
            metro->edges[i-1] = edge;
        }
        FillContext(metro);
    }
    return TRUE;
}


void FillContext(Metro* metro) {
    int num = metro->stationNum;
    for (int i = 1; i <= num; i++) {
        Station* curStation = metro->stations[i];
        for (int j = 0; j < curStation->metroNum; j++) {
            MetroContext* curContext = curStation->metroContexts[j];
            if (curContext->metro == metro) {
                curContext->nextStation = metro->stations[i+1];
                curContext->lastStation = metro->stations[i-1];
                curContext->nextEdge = metro->edges[i];
                curContext->lastEdge = metro->edges[i-1];
                break;
            }
        }
    }
}


