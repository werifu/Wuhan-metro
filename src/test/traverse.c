//
// Created by chen on 3/1/21.
//
#include "stdio.h"
#include "traverse.h"
#include "../models/models.h"
#include "../models/common.h"
#include "../utils/hash.h"
extern MetroSystem* metroSystem;


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


void DFS_system(station) {

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