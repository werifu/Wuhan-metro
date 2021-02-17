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

    return 0;
}
