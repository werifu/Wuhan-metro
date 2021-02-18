#include "models.h"
#include "stdio.h"
#include "hash.h"
extern MetroSystem* metroSystem;
extern StationTable stationTable;
extern MetroTable metroTable;
int main() {
    InitMetroSystem();
    InitStationTable();
    InitMetroTable();
//    Metro* m = NewMetro("name_metro",10);
//    Station* a = NewStation("name_a", m, 0, 1, 1);
//    Station* b = NewStation("name_b", m, 1, 0, 2);
//    InsertStation(stationTable, a);
//    InsertStation(stationTable, b);
//    Station* result = GetStation(stationTable, "name_b");

    LoadMetros();
    LoadStations();
    LoadEdgesAndContexts();
    Metro* metro = GetMetro(metroTable, "1号线");
//    Station* station = GetStation(stationTable, "天河机场");
    if (metro) {
        printf("%s, %d\n",metro->name, metro->stationNum);
    }
    return 0;
}
