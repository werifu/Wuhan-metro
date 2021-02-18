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

int InitMetroSystem() {
    metroSystem = (MetroSystem*)malloc(sizeof(MetroSystem));
    metroSystem->metroNum = 0;
    return TRUE;
}

int InitStationTable() {
    stationTable = (StationTable)malloc(sizeof(StationHash*) * STATION_TABLE_LEN);
    for (int i = 0; i < STATION_TABLE_LEN; i++) {
        stationTable[i] = (StationHash*)malloc(sizeof(StationHash));
        stationTable[i]->cur = NULL;
        stationTable[i]->next = NULL;
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

Metro* NewMetro(char* name, int length, int capacity) {
    Metro* metro = (Metro*)malloc(sizeof(Metro));
    strcpy(metro->name, name);
    metro->length = length;
    metro->stationNum = 0;
    metro->capacity = capacity;
    return metro;
}


Station* NewStation(char* name, Metro* metro, int posX, int posY, int id) {
    Station* station = GetStation(stationTable, name);
    if (!station) {
        station = (Station *) malloc(sizeof(Station));
        station->metroNum = 0;
        strcpy(station->name, name);
        station->posX = posX;
        station->posY = posY;
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


Edge* NewEdge(Station* startStation, Station* endStation, Metro* metro, TrafficFlow* trafficFlow, int length, int timeNeeded) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->startStation = startStation;
    edge->endStation = endStation;
    edge->metro = metro;
    edge->trafficFlow = trafficFlow;
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
    int length;
    int capacity;
    while (fgets(line, 1024, fp)) {
        const char* tok = strtok(line, ",");
        strcpy(name, tok);
        tok = strtok(NULL,",");
        capacity = atoi(tok);
        tok = strtok(NULL,",");
        length = atoi(tok);
        Metro* metro = NewMetro(name, length, capacity);

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
    int posX, posY, id;
    char metro_name[MAX_METRO_NAME_LEN];
    while (fgets(line, 1024, fp)) {
        const char* tok = strtok(line, ",");
        strcpy(name, tok);

        tok = strtok(NULL,",");
        posX = atoi(tok);

        tok = strtok(NULL,",");

        posY = atoi(tok);

        tok = strtok(NULL, ",");
        strcpy(metro_name, tok);
        Metro* metro = GetMetro(metroTable, metro_name);
        if (!metro) {
            fprintf(stderr, "Metro %s Not Found\n", metro_name);
            continue;
        }

        tok = strtok(NULL, ",");
        id = atoi(tok);

        Station* station = NewStation(name, metro, posX, posY, id);
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
        int all_length = metro->length;
        int num = metro->stationNum;
        if (num == 0) {
            fprintf(stderr, "Metro %s has 0 station\n", metro->name);
            continue;
        }
        int length = all_length/num;
        for (int i = 2; i <= num; i++) {
            Edge* edge = NewEdge(metro->stations[i-1], metro->stations[i], metro, NULL, length, TWO_STATIONS_RUN_TIME);
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


