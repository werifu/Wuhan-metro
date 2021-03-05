//
// Created by chen on 2/18/21.
//

#include "paths.h"
#include "malloc.h"
#include "float.h"
#include "../models/models.h"
#include "../models/stations.h"
#include "../test/traverse.h"
#include "string.h"
#include "metro_time.h"
#include "../models/trafficFlows.h"
extern MetroSystem* metroSystem;

// Time: O(n^3)
Path* GetShortestPath(Station* src,Station* dest, timestamp_t startTime) {
    ResetStatus();
    Station* nextKnownNode = src;
    Metro* cur_metro = NULL;
    Path* result = (Path*)malloc(sizeof(Path));

    int knownNum = 0;
    Station* knownNodes[MAX_ALL_STATION_NUM];

    src->costToHere = 0;
    src->arrivalTime = startTime;
    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        nextKnownNode->known = TRUE;
        knownNodes[knownNum++] = nextKnownNode;
//        PrintCurStationArrivalTime(nextKnownNode);
        // get the next node
        double min_cost = DBL_MAX;
        // At the src there is no cur_metro
        Station* mayNext = UpdateAdjStatusUponLen(src, NULL);
        if (mayNext && mayNext->costToHere < min_cost) {
            min_cost = mayNext->costToHere;
            nextKnownNode = mayNext;
        }
        for (int j = 1; j < knownNum; j++) {
            mayNext = UpdateAdjStatusUponLen(knownNodes[j], cur_metro);
            if (!mayNext) continue;
            if (mayNext->costToHere < min_cost) {
                min_cost = mayNext->costToHere;
                nextKnownNode = mayNext;
            }
        }
    }
    printf("Shortest, Arrival time: %0.2f. ", dest->arrivalTime);
    if (dest->arrivalTime > TWENTY_THREE_CLOCK_TIMESTAMP) {
        return result;
    }
    SetPathFromDest(result, dest);

    // cannot arrive

    PrintFromDest(dest);
    return result;
}


Path* GetLeastTimePath(Station* src, Station* dest, timestamp_t startTime) {
    ResetStatus();
    Station* nextKnownNode = src;
    Metro* cur_metro = NULL;
    Path* result = (Path*)malloc(sizeof(Path));

    int knownNum = 0;
    Station* knownNodes[MAX_ALL_STATION_NUM];

    src->costToHere = 0;
    src->arrivalTime = startTime;

    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        nextKnownNode->known = TRUE;
        knownNodes[knownNum++] = nextKnownNode;
//        PrintCurStationArrivalTime(nextKnownNode);
        // get the next node
        double min_cost = DBL_MAX;

        // At the src there is no cur_metro
        Station* mayNext = UpdateAdjStatusUponTime(src, NULL);
        if (mayNext && mayNext->costToHere < min_cost) {
            min_cost = mayNext->costToHere;
            nextKnownNode = mayNext;
        }
        for (int j = 1; j < knownNum; j++) {
            cur_metro = GetEdgeBetweenAdj(knownNodes[j]->last_node, knownNodes[j])->metro;
            mayNext = UpdateAdjStatusUponTime(knownNodes[j], cur_metro);
            if (!mayNext) continue;
            if (mayNext->costToHere < min_cost) {
                min_cost = mayNext->costToHere;
                nextKnownNode = mayNext;
            }
        }
    }
    printf("Fast, Arrival time: %0.2f. ", dest->arrivalTime);
    SetPathFromDest(result, dest);
    PrintFromDest(dest);
    return result;
}

Path* GetAvoidingCrowdPath(Station* src, Station* dest, timestamp_t startTime) {
    ResetStatus();
    Station* nextKnownNode = src;
    Metro* cur_metro = NULL;
    Path* result = (Path*)malloc(sizeof(Path));

    int knownNum = 0;
    Station* knownNodes[MAX_ALL_STATION_NUM];

    src->costToHere = 0;
    src->arrivalTime = startTime;

    int n = metroSystem->stationNum;
    for (int i = 0; i < n; i++) {
        nextKnownNode->known = TRUE;
        knownNodes[knownNum++] = nextKnownNode;

        // get the next node
        double min_cost = DBL_MAX;

        // At the src there is no cur_metro
        Station* mayNext = UpdateAdjStatusUponTime(src, NULL);
        if (mayNext && mayNext->costToHere < min_cost) {
            min_cost = mayNext->costToHere;
            nextKnownNode = mayNext;
        }
        for (int j = 1; j < knownNum; j++) {
            cur_metro = GetEdgeBetweenAdj(knownNodes[j]->last_node, knownNodes[j])->metro;
            mayNext = UpdateAdjStatusUponTime(knownNodes[j], cur_metro);
            if (!mayNext) continue;
            if (mayNext->costToHere < min_cost) {
                min_cost = mayNext->costToHere;
                nextKnownNode = mayNext;
            }
        }
    }
    printf("Avoid Crowded, Arrival time: %0.2f. ", dest->arrivalTime);
    SetPathFromDest(result, dest);
    PrintFromDest(dest);
    return result;
}

// Returns next node that may be known
// Should check if NULL
Station* UpdateAdjStatusUponLen(Station* station, Metro* cur_metro) {
    double min_cost = DBL_MAX;
    Station* next = NULL;
    double originCost = station->costToHere;
    int n = station->metroNum;
    for (int i = 0; i < n; i++) {
        MetroContext* ctx = station->metroContexts[i];

        if (ctx->nextStation && ctx->nextStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->nextEdge->metro->trafficFlow, station->arrivalTime));
            if (coeff != DBL_MAX) {
                double timeNeeded = (ctx->nextEdge->timeNeeded +  STATION_STOP_TIME) * coeff;
                if (ctx->nextEdge->metro != cur_metro && cur_metro != NULL) timeNeeded += TRANSFER_TIME;
                double new_cost = ctx->nextEdge->length + originCost;
                if (new_cost < ctx->nextStation->costToHere) {
                    ctx->nextStation->costToHere = new_cost;
                    ctx->nextStation->last_node = station;
                    ctx->nextStation->arrivalTime = station->arrivalTime + timeNeeded;
                }
                if (ctx->nextStation->costToHere < min_cost) {
                    next = ctx->nextStation;
                    min_cost = ctx->nextStation->costToHere;
                }
            }
        }
        if (ctx->lastStation && ctx->lastStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->lastEdge->metro->trafficFlow, station->arrivalTime));
            if (coeff != DBL_MAX) { // cannot go this way
                double timeNeeded = (ctx->lastEdge->timeNeeded +  STATION_STOP_TIME) * coeff;
                if (ctx->lastEdge->metro != cur_metro && cur_metro != NULL) timeNeeded += TRANSFER_TIME;
                double new_cost = ctx->lastEdge->length + originCost;
                if (new_cost < ctx->lastStation->costToHere) {
                    ctx->lastStation->costToHere = new_cost;
                    ctx->lastStation->last_node = station;
                    ctx->lastStation->arrivalTime = station->arrivalTime + timeNeeded;
                }
                if (ctx->lastStation->costToHere < min_cost) {
                    next = ctx->lastStation;
                    min_cost = ctx->lastStation->costToHere;
                }
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
            if (coeff != DBL_MAX) {
                double new_cost = originCost + (ctx->nextEdge->timeNeeded +  STATION_STOP_TIME) * coeff;
                // transfer station needs more time
                // cur_metro is NULL when the node is the src
                if (ctx->nextEdge->metro != cur_metro && cur_metro != NULL) new_cost += TRANSFER_TIME;
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
        }
        if (ctx->lastStation && ctx->lastStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->lastEdge->metro->trafficFlow, station->arrivalTime));
            if (coeff != DBL_MAX) { // cannot go this way
                double new_cost = originCost + (ctx->lastEdge->timeNeeded + STATION_STOP_TIME) * coeff;

                if (ctx->lastEdge->metro != cur_metro && cur_metro != NULL) new_cost += TRANSFER_TIME;
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
    }
    return next;
}


Station* UpdateAdjStatusUponTimeWithCrowd(Station* station, Metro* cur_metro) {
    double min_cost = DBL_MAX;
    Station* next = NULL;
    double originCost = station->costToHere;
    int n = station->metroNum;
    for (int i = 0; i < n; i++) {
        MetroContext* ctx = station->metroContexts[i];
        if (ctx->nextStation && ctx->nextStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->nextEdge->metro->trafficFlow, station->arrivalTime));
            if (coeff != DBL_MAX) {
                double new_cost = originCost + (ctx->nextEdge->timeNeeded +  STATION_STOP_TIME) * coeff;
                // transfer station needs more time
                // cur_metro is NULL when the node is the src
                if (ctx->nextEdge->metro != cur_metro && cur_metro != NULL) new_cost += TRANSFER_TIME;
                // if too crowded there is cost penalty
                if (new_cost + ((coeff > 1)?CROWD_PENALTY:0) < ctx->nextStation->costToHere) {
                    ctx->nextStation->costToHere = new_cost;
                    ctx->nextStation->last_node = station;
                    ctx->nextStation->arrivalTime = station->arrivalTime + new_cost - originCost;
                }
                if (ctx->nextStation->costToHere < min_cost) {
                    next = ctx->nextStation;
                    min_cost = ctx->nextStation->costToHere;
                }
            }
        }
        if (ctx->lastStation && ctx->lastStation->known == FALSE) {
            double coeff = GetTimeCoefficient(GetCrowded(ctx->lastEdge->metro->trafficFlow, station->arrivalTime));
            if (coeff != DBL_MAX) {
                // penalty
                double new_cost = originCost + (ctx->lastEdge->timeNeeded + STATION_STOP_TIME) * coeff;

                if (ctx->lastEdge->metro != cur_metro && cur_metro != NULL) new_cost += TRANSFER_TIME;
                if (new_cost  + ((coeff > 1)?CROWD_PENALTY:0) < ctx->lastStation->costToHere) {
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

int CanReach(Station* dest) {
    return GetPathEdgeNumFromDest(dest) == 0;
}

