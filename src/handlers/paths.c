//
// Created by chen on 2/18/21.
//

#include "paths.h"
#include "malloc.h"
#include "float.h"
#include "../models/models.h"
#include "../test/traverse.h"
#include "string.h"
#include "metro_time.h"
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
            Station* mayNext = UpdateAdjStatusUponLen(knownNodes[j]);
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


Path* GetLeastTimePath(Station* src, Station* dest, time_t startTime) {
    Station* nextKnownNode = src;
    Path* result = (Path*)malloc(sizeof(Path));

    int knownNum = 0;
    Station* knownNodes[MAX_ALL_STATION_NUM];

    src->costToHere = 0;
    src->arrivalTime = startTime;

    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        nextKnownNode->known = TRUE;
        knownNodes[knownNum++] = nextKnownNode;


    }
    return result;
}
// Returns next node that may be known
// Should check if NULL
Station* UpdateAdjStatusUponLen(Station* station) {
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

Station* UpdateAdjStatusUponTime(Station* station, Metro* cur_metro) {
    double min_cost = DBL_MAX;
    Station* next = NULL;
    double originCost = station->costToHere;
    int n = station->metroNum;
    for (int i = 0; i < n; i++) {
        MetroContext* ctx = station->metroContexts[i];
        if (ctx->nextStation && ctx->nextStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->nextEdge->metro->trafficFlow, station->arrivalTime));
            double new_cost = originCost + (ctx->nextEdge->timeNeeded +  STATION_STOP_TIME) * coeff;
            // transfer station needs more time
            if (ctx->nextEdge->metro != cur_metro) new_cost += TRANSFER_TIME;
            if (new_cost < ctx->nextStation->costToHere) {
                ctx->nextStation->costToHere = new_cost;
                ctx->nextStation->last_node = station;
                ctx->nextStation->arrivalTime = station->arrivalTime + new_cost - originCost;
            }
            if (ctx->nextStation->costToHere < min_cost) {
                next = ctx->nextStation;
                min_cost = ctx->nextStation->costToHere;
            }
        }
        if (ctx->lastStation && ctx->lastStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->lastEdge->metro->trafficFlow, station->arrivalTime));
            double new_cost = originCost + (ctx->lastEdge->timeNeeded +  STATION_STOP_TIME) * coeff;

            if (ctx->lastEdge->metro != cur_metro) new_cost += TRANSFER_TIME;
            if (new_cost < ctx->lastStation->costToHere) {
                ctx->lastStation->costToHere = new_cost;
                ctx->lastStation->last_node = station;
                ctx->lastStation->arrivalTime = station->arrivalTime + new_cost - originCost;
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
    int n = path->edgeNum;
    if (n <= 1) return 0;
    Edge* e = path->edgesOnPath[0];
    Metro* curMetro = e->metro;
    for (int i = 1; i < n; i++) {
        e = path->edgesOnPath[i];
        if (e->metro != curMetro) {
            result++;
            curMetro = e->metro;
        }
    }
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
            fprintf(stderr, "Cannot find an edge between two adjacent vertexes: %s, %s\n", dest->name, dest->last_node->name);
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