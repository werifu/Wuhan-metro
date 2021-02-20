//
// Created by chen on 2/17/21.
//

#include "hash.h"
#include "common.h"
#include "malloc.h"
// s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
unsigned int get_string_hash(char* name) {
    int n = (int)strlen(name);
    if (n == 0) return 0;
    unsigned int hash = 0;
    unsigned int t31[n];
    t31[0] = 1;
    for (int i = 1; i < n; i++) {
        t31[i] = (t31[i-1]<<5) - t31[i-1];  // t31[i-1]*31
    }
    for (int i = 0; i < n; i++) {
        hash += name[i]*t31[n-i-1];
    }
    return hash;
}

Station* GetStation(StationTable stationTable,char* name) {
    if (!name) return NULL;
    unsigned int hash = get_string_hash(name);
    hash = hash % STATION_TABLE_LEN;
    StationHash* curHash = stationTable[hash];

    while (curHash) {
        if (curHash->cur && strcmp(name, curHash->cur->name) == 0) {
            return curHash->cur;
        }
        curHash = curHash->next;
    }
    return NULL;
}

int InsertStation(StationTable stationTable, Station* station) {
    if (!station) return FALSE;
    unsigned int hash = get_string_hash(station->name);
    hash = hash % STATION_TABLE_LEN;
    StationHash* curHash = stationTable[hash];
    if (!curHash->cur) {
        curHash->cur = station;
        return TRUE;
    }
    while (curHash->next) {
        curHash = curHash->next;
    }
    StationHash* newHash = (StationHash*)malloc(sizeof(StationHash));
    newHash->cur = station;
    newHash->next = NULL;
    curHash->next = newHash;
    return TRUE;
}

Metro* GetMetro(MetroTable metroTable, char* name) {
    if (!name) return NULL;
    unsigned int hash = get_string_hash(name);
    hash = hash % METRO_TABLE_LEN;
    MetroHash* curHash = metroTable[hash];
    while (curHash) {
        if (curHash->cur && strcmp(name, curHash->cur->name) == 0) {
            return curHash->cur;
        }
        curHash = curHash->next;
    }
    return NULL;
}

int InsertMetro(MetroTable metroTable, Metro* metro) {
    if (!metro) return FALSE;
    unsigned int hash = get_string_hash(metro->name);
    hash = hash % METRO_TABLE_LEN;
    MetroHash* curHash = metroTable[hash];
    if (!curHash->cur) {
        curHash->cur = metro;
        return TRUE;
    }
    while (curHash->next) {
        curHash = curHash->next;
    }
    MetroHash* newHash = (MetroHash*)malloc(sizeof(MetroHash));
    newHash->cur = metro;
    newHash->next = NULL;
    curHash->next = newHash;
    return TRUE;
}