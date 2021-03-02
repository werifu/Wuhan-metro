//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_STATIONS_H
#define WUHAN_METRO_STATIONS_H

#include "models.h"

// Inits the hash table storing the all stations
// Returns TRUE if ok, PANIC if cannot allocate
// Memory: sizeof(StationTable*)*STATION_TABLE_LEN
int InitStationTable();


// Loads basic info of metros from "data/stations.csv"
// Format of csv: station_name,longitude,latitude,metro_name,id
// Returns TRUE if ok, returns ERROR if cannot open the file
// Memory: Allocates sizeof(Station) * Number of individual stations in the .csv (not duplicate)
int LoadStations();


// Returns a new Station filled with name & length. No MetroContext in it. Returns NULL if failing.
//
// Params: 1.(char*) the name of the station;
//         2.(Metro*) the metro the station is on;
//         3&4.(double)
//         5.(int) id of the station on the current metro.
// Memory: Allocates sizeof(Station)+sizeof(StationContext) when there is no this station. Otherwise allocates sizeof(MetroContext).
Station* NewStation(char* name, Metro* metro, double longitude, double latitude, int id);



int LoadTransferStationGraph();



TransferStationNode* NewTransferNode();



#endif //WUHAN_METRO_STATIONS_H
