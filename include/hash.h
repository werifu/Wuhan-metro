//
// Created by chen on 2/17/21.
// Provided a Hash Table for the stations created
//
#include "models.h"
#include "string.h"
#ifndef WUHAN_METRO_STATION_HASH_H
#define WUHAN_METRO_STATION_HASH_H


#define STATION_TABLE_LEN 500
#define METRO_TABLE_LEN 50
//==================================

// Stores all the pointers of stations.
// Needed to be allocated.
typedef struct StationHash StationHash;
typedef StationHash** StationTable;

struct StationHash {
    Station* cur;
    StationHash* next;
};


// Returns the station found via its name. Returns NULL if not found.
Station* GetStation(StationTable ,char* name);

// Inserts a new station into the stationTable;
// Returns TRUE if ok, otherwise FALSE
// No extra memory is allocated
int InsertStation(StationTable, Station*);


//==================================

typedef struct MetroHash MetroHash;
typedef MetroHash** MetroTable;

struct MetroHash {
    Metro* cur;
    MetroHash* next;
};


// Returns the metro found via its name. Returns NULL if not found.
Metro* GetMetro(MetroTable, char* name);

// Inserts a new metro into the metroTable;
// Returns TRUE if ok, otherwise FALSE
// No extra memory is allocated
int InsertMetro(MetroTable, Metro*);


//==================================

// Returns the hash of a string
// Should not pass a NULL param
unsigned int get_string_hash(char*);


#endif //WUHAN_METRO_STATION_HASH_H
