//
// Created by chen on 3/1/21.
//
#include "stdio.h"
#include "traverse.h"

#include "../models/common.h"
#include "../utils/hash.h"
extern MetroSystem* metroSystem;
extern TransferStationGraph* tsfStationGraph;

int TraverseSystem() {
    int n = metroSystem->metroNum;
    for (int i = 0; i < n; i++) {
        Metro* metro = metroSystem->metros[i];
        int n = metro->stationNum;
        printf("%s, 全长%.2fkm，有%d站点\n", metro->name, metro->length, metro->stationNum);
        for (int j = 1; j < n; j++) {
            printf("%s->", metro->stations[j]->name);
        }
        printf("%s\n", metro->stations[n]->name);
    }
    return TRUE;
}


void DFS_System(Station* station) {
    if (!station || station->known) return;
    printf("%s\n", station->name);
    station->known = TRUE;
    for (int i = 0; i < station->metroNum; i++) {
        MetroContext* ctx = station->metroContexts[i];
        DFS_System(ctx->nextStation);
        DFS_System(ctx->lastStation);
    }
}


int TraverseMetroViaEdge(char* metroName) {
    Metro* metro = GetMetro(metroName);
    if (!metro) return FALSE;
    int n = metro->stationNum;
    if (n == 0) return FALSE;
    Station* cur = metro->stations[1];
    while (cur) {
        printf("%s", cur->name);
        for (int i = 0; i < cur->metroNum; i++) {
            MetroContext* ctx = cur->metroContexts[i];
            if (strcmp(metroName, ctx->metro->name) == 0) {
                if (ctx->nextEdge == NULL) return TRUE;
                cur = ctx->nextEdge->endStation;
                printf("--%.2f-->", ctx->nextEdge->length);
                break;
            }
        }
    }
    return TRUE;
}


void PrintGraph() {
    int n = tsfStationGraph->tsfStationNum;
    for (int i = 0; i < n; i++) {
        TransferStationNode* node = tsfStationGraph->tsfStationNodes[i];
        printf("station:%s, it's on ", node->station->name);
        int metroNum = node->station->metroNum;
        for (int j = 0; j < metroNum; j++) {
            printf("%s, ", node->station->metroContexts[j]->metro->name);
        }
        printf("its adj vertexes are: ");
        for (int j = 0; j < node->adjNum; j++) {
            printf("%s,", node->adjNodes[j]->station->name);
        }
        printf("\n");
    }
}


void PrintAdjVertex() {
    int n = metroSystem->stationNum;
    printf("There are %d stations:\n", n);
    for (int i = 0; i < n; i++) {
        Station* station = metroSystem->stations[i];
        printf("%s\t adj vertexes: ", station->name);
        int m = station->metroNum;
        for (int j = 0; j < m; j++) {
            MetroContext* ctx = station->metroContexts[j];
            char last[30];
            strcpy(last, ctx->lastStation? ctx->lastStation->name: "不存在");
            char next[30];
            strcpy(next, ctx->nextStation? ctx->nextStation->name: "不存在");
            printf("%s,%s,", last, next);
        }
        printf("\n");
    }
}

void PrintFromDest(Station* station) {
    while (station->last_node) {
        printf("%s<-", station->name);
        station = station->last_node;
    }
    printf("%s\n", station->name);
}