//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_METROS_H
#define WUHAN_METRO_METROS_H
#include "metros.h"
#include "models.h"

// Inits the main Metro System. Returns the status(TRUE, FALSE, ERROR or PANIC) of the operation. Loads the data into the extern variable metroSystem defined in models.c
//
// Memory: Allocates sizeof(MetroSystem)
int InitMetroSystem();


// Inits the hash table storing the all stations
// Returns TRUE if ok, PANIC if cannot allocate
// Memory: sizeof(MetroTable*)*METRO_TABLE_LEN
int InitMetroTable();


// Loads basic info of metros from "data/metros.csv"
// Format of csv: metro_name,capacity,length,station_num
// Returns TRUE if ok, returns ERROR if cannot open the file
// Memory: Allocates sizeof(Metro) * Number of metros in the .csv
int LoadMetros();


// Returns a new Metro filled with name & length. No stations and edges in it.
//
// Params:  1.(char*) the name of the metro;
//          2.(int) the physical length of the metro.
//          3.(int)passenger capacity of the metro.
//          4.(TrafficFlow*)
// Memory: Allocates sizeof(Metro)
Metro* NewMetro(char* name, double length, int capacity, TrafficFlow* trafficFlow);

#endif //WUHAN_METRO_METROS_H
