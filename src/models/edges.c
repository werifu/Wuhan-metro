//
// Created by chen on 3/1/21.
//

#include "edges.h"
#include "malloc.h"
#include "common.h"
#include "string.h"
#include "models.h"
#include "metroContexts.h"
#include "../utils/hash.h"


extern MetroTable metroTable;

Edge* NewEdge(Station* startStation, Station* endStation, Metro* metro, double length, double timeNeeded) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->startStation = startStation;
    edge->endStation = endStation;
    edge->metro = metro;
    edge->length = length;
    edge->timeNeeded = timeNeeded;
    return edge;
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
