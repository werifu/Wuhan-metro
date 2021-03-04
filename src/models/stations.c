//
// Created by chen on 3/1/21.
//

#include "stations.h"
#include "../utils/hash.h"
#include "malloc.h"
#include "common.h"
#include "metroContexts.h"
#include "stdlib.h"
#include "float.h"
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

        // put the station into the system list if not exists
        if (station->known == FALSE) {
            metroSystem->stations[metroSystem->stationNum++] = station;
            station->known = TRUE;
        }
    }
    ResetStatus();
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
        station->known = FALSE;
    }
    MetroContext* context = NewMetroContext(metro, id);
    station->metroContexts[station->metroNum] = context;
    station->metroNum++;

    return station;
}


int ResetStatus() {
    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        Station* station = metroSystem->stations[i];
        station->known = FALSE;
        station->costToHere = DBL_MAX;
        station->last_node = NULL;
        station->arrivalTime = 0;
    }
    return TRUE;
}

//
//// Used to help make a graph served for path
//typedef struct tmpTransfers tmpTransfers;
//struct tmpTransfers {
//    Metro* metro;
//    int tsfStationNum;
//    TransferStationNode* tsfStationNodes[MAX_STATION_NUM_LEN];
//};
//
//int LoadTransferStationGraph() {
//    //init the graph
//    tsfStationGraph = NewGraph();
//
//    tmpTransfers* tmp_transfers_arr[MAX_METRO_NUM_LEN];
//    // adj table of each metro
//    int metroNum = metroSystem->metroNum;
//    for (int i = 0; i < metroNum; i++) {
//        Metro* metro = metroSystem->metros[i];
//        tmp_transfers_arr[i] = (tmpTransfers*)malloc(sizeof(tmpTransfers));
//        tmp_transfers_arr[i]->metro = metro;
//        tmp_transfers_arr[i]->tsfStationNum = 0;
//
//        int stationNum = metro->stationNum;
//        for (int id = 1; id <= stationNum; id++) {
//            Station* station = metro->stations[id];
//            if (station->metroNum == 1) continue;
//            TransferStationNode* targetNode = get_station_node_in_graph(station);
//            // put the node into the adj table of metros
//            tmp_transfers_arr[i]->tsfStationNodes[tmp_transfers_arr[i]->tsfStationNum++] = targetNode;
//        }
//    }
//
//    // load the adj vertexes for all
//    for (int i = 0; i < metroNum; i++) {
//        int tsfStationNumPerMetro = tmp_transfers_arr[i]->tsfStationNum;
//        for (int j = 0; j < tsfStationNumPerMetro; j++) {
//            for (int k = 0; k < tsfStationNumPerMetro; k++) {
//                if (j == k) continue;
//                // load stations on the same metro as adj.
//                int adjNum = tmp_transfers_arr[i]->tsfStationNodes[j]->adjNum++;
//                tmp_transfers_arr[i]->tsfStationNodes[j]->adjNodes[adjNum] = tmp_transfers_arr[i]->tsfStationNodes[k];
//            }
//        }
//    }
//
//
//    return TRUE;
//}
//
//TransferStationNode* get_station_node_in_graph(Station* station) {
//    // if transfer station:
//    int tsfNum = tsfStationGraph->tsfStationNum;
//    int found_flag = FALSE;
//    TransferStationNode* targetNode;
//    for (int j = 0; j < tsfNum; j++) {
//        if (station == tsfStationGraph->tsfStationNodes[j]->station) {
//            found_flag = TRUE;
//            targetNode = tsfStationGraph->tsfStationNodes[j];
//            break;
//        }
//    }
//    // the graph has no this station node, we should new one node and put the node into the graph
//    if (!found_flag) {
//        targetNode = NewTransferNode(station);
//        tsfStationGraph->tsfStationNodes[tsfStationGraph->tsfStationNum++] = targetNode;
//    }
//    return targetNode;
//}
//
//
//TransferStationGraph* NewGraph() {
//    return (TransferStationGraph*)malloc(sizeof(TransferStationGraph));
//}
//
//TransferStationNode* NewTransferNode(Station* station) {
//    TransferStationNode* targetNode = (TransferStationNode*)malloc(sizeof(TransferStationNode));
//    targetNode->station = station;
//    targetNode->adjNum = 0;
//    return targetNode;
//}

