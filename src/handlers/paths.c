//
// Created by chen on 2/18/21.
//

#include "paths.h"
#include "malloc.h"
#include "float.h"
#include "../models/models.h"
#include "../test/traverse.h"
#include "string.h"
extern MetroSystem* metroSystem;

// Time: O(n^3)
Path* GetShortestPath(Station* src,Station* dest) {
    Station* nextKnownNode = src;
    Path* result = (Path*)malloc(sizeof(Path));

    int knownNum = 0;
    Station* knownNodes[MAX_ALL_STATION_NUM];

    src->costToHere = 0;
    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        nextKnownNode->known = TRUE;
        knownNodes[knownNum++] = nextKnownNode;
        // get the next node
        double min_cost = DBL_MAX;
        for (int j = 0; j < knownNum; j++) {
            Station* mayNext = UpdateAdjStatus(knownNodes[j]);
            if (!mayNext) continue;
            if (mayNext->costToHere < min_cost) {
                min_cost = mayNext->costToHere;
                nextKnownNode = mayNext;
            }
        }
    }
    SetPathFromDest(result, dest);
    return result;
}

// Returns next node that may be known
// Should check if NULL
Station* UpdateAdjStatus(Station* station) {
    double min_cost = DBL_MAX;
    Station* next = NULL;
    double originCost = station->costToHere;
    int n = station->metroNum;
    for (int i = 0; i < n; i++) {
        MetroContext* ctx = station->metroContexts[i];

        if (ctx->nextStation && ctx->nextStation->known == FALSE) {
            double new_cost = ctx->nextEdge->length + originCost;
            if (new_cost < ctx->nextStation->costToHere) {
                ctx->nextStation->costToHere = new_cost;
                ctx->nextStation->last_node = station;
            }
            if (ctx->nextStation->costToHere < min_cost) {
                next = ctx->nextStation;
                min_cost = ctx->nextStation->costToHere;
            }
        }
        if (ctx->lastStation && ctx->lastStation->known == FALSE) {
            double new_cost = ctx->lastEdge->length + originCost;
            if (new_cost < ctx->lastStation->costToHere) {
                ctx->lastStation->costToHere = new_cost;
                ctx->lastStation->last_node = station;
            }
            if (ctx->lastStation->costToHere < min_cost) {
                next = ctx->lastStation;
                min_cost = ctx->lastStation->costToHere;
            }
        }
    }
    return next;
}


int GetTransferNum(Path* path) {
    int result = 0;
    return result;
}

int GetPathEdgeNumFromDest(Station* station) {
    int result = 0;
    while (station->last_node) {
        result++;
        station = station->last_node;
    }
    return result;
}

int SetPathFromDest(Path* result, Station* dest) {
    int edgeNum = GetPathEdgeNumFromDest(dest);
    result->edgeNum = edgeNum;
    for (int i = edgeNum-1; i >= 0; i--) {
        Edge* targetEdge = GetEdgeBetweenAdj(dest, dest->last_node);
        if (!targetEdge) {
            fprintf(stderr, "Cannot find a target between two adjacent vertexes.\n");
            return ERROR;
        }
        result->edgesOnPath[i] = targetEdge;
        result->length += targetEdge->length;
    }
    return TRUE;
}

Edge* GetEdgeBetweenAdj(Station* station1, Station* station2) {
    Edge* result = NULL;
    for (int i = 0; i < station1->metroNum; i++) {
        MetroContext* ctx = station1->metroContexts[i];
        if (ctx->nextStation == station2) {
            result = ctx->nextEdge;
            break;
        } else if (ctx->lastStation == station2){
            result = ctx->lastEdge;
            break;
        }
    }
    return result;
}