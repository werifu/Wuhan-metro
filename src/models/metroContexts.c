//
// Created by chen on 3/1/21.
//
#include "metroContexts.h"
#include "malloc.h"

MetroContext* NewMetroContext(Metro* metro, int id) {
    MetroContext* newContext = (MetroContext*)malloc(sizeof(MetroContext));
    newContext->id = id;
    newContext->metro = metro;
    return newContext;
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
