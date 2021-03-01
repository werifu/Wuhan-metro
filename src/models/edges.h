//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_EDGES_H
#define WUHAN_METRO_EDGES_H
#include "models.h"


// Called after LoadMetro() and LoadStations()
// Along all metros fills the edges and contexts of the stations.
// Returns TRUE if ok
// Loads metro data from "data/metros.csv"
int LoadEdgesAndContexts();


// Returns a new Edge filled with all properties.
//
// NULL: pointers variable can be NULL
// Memory: Allocates sizeof(Edge)
Edge* NewEdge(Station* startStation, Station* endStation, Metro* metro, double length, double timeNeeded);

#endif //WUHAN_METRO_EDGES_H
