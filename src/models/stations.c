//
// Created by chen on 3/1/21.
//

#include "stations.h"
#include "../utils/hash.h"
#include "malloc.h"
#include "common.h"
#include "metroContexts.h"
#include "stdlib.h"

extern StationTable stationTable;
extern MetroTable metroTable;


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
        Metro* metro = GetMetro(metro_name);
        if (!metro) {
            fprintf(stderr, "Metro %s Not Found\n", metro_name);
            continue;
        }

        tok = strtok(NULL, ",");
        id = atoi(tok);

        Station* station = NewStation(name, metro, longitude, latitude, id);
        if (!GetStation(station->name)) {
            InsertStation(station);
        }
        metro->stationNum++;

        // id begins with 1 (This system only this one begins with 1)
        metro->stations[id] = station;
    }
    return TRUE;
}


Station* NewStation(char* name, Metro* metro, double longitude, double latitude, int id) {
    Station* station = GetStation(name);
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