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
extern TransferStationGraph* tsfStationGraph;

extern MetroSystem* metroSystem;
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


typedef struct tmpTransfers tmpTransfers;
struct tmpTransfers {
    Metro* metro;
    int tsfStationNum;
    Station* tsfStations[MAX_STATION_NUM_LEN];
};

int LoadTransferStationGraph() {
    tmpTransfers* tmp_transfers_arr[MAX_METRO_NUM_LEN];
    // adj table of each metro
    int metroNum = metroSystem->metroNum;
    for (int i = 0; i < metroNum; i++) {
        Metro* metro = metroSystem->metros[i];
        tmp_transfers_arr[i] = (tmpTransfers*)malloc(sizeof(tmpTransfers));
        tmp_transfers_arr[i]->metro = metro;
        tmp_transfers_arr[i]->tsfStationNum = 0;

        int stationNum = metro->stationNum;
        for (int id = 1; id <= stationNum; id++) {
            Station* station = metro->stations[id];
            if (station->metroNum == 1) continue;
            tmp_transfers_arr[i]->tsfStations[tmp_transfers_arr[i]->tsfStationNum++] = station;
        }
    }

//    for (int i = 0; i < metroNum; i++) {
//        int n = tmp_transfers_arr[i]->tsfStationNum;
//        for (int j = 0; j < n; j++) {
//
//        }
//    }
    return TRUE;
}

