#include "models.h"
#include "stdio.h"
#include "hash.h"
#include "test.h"
extern MetroSystem* metroSystem;
extern StationTable stationTable;
extern MetroTable metroTable;


int main() {
    InitMetroSystem();
    InitStationTable();
    InitMetroTable();

    LoadMetros();
    LoadStations();
    LoadEdgesAndContexts();
    TestGetStation();
    return 0;
}
