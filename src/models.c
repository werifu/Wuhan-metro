//
// Created by chen on 2/16/21.
//

#include "models.h"
#include "malloc.h"
#include "common.h"
#include "hash.h"
#include "string.h"

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

Metro* NewMetro(char* name, int length) {
    Metro* metro = (Metro*)malloc(sizeof(Metro));
    strcpy(metro->name, name);
    metro->length = length;
    metro->stationNum = 0;
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