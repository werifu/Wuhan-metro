#include "models/models.h"
#include "models/metros.h"
#include "models/stations.h"
#include "models/edges.h"
#include "models/trafficFlows.h"
#include "utils/hash.h"


extern MetroSystem* metroSystem;
extern StationTable stationTable;
extern MetroTable metroTable;


int main() {
    InitMetroSystem();
    InitStationTable();
    InitMetroTable();
    InitTrafficFlowTable();

    LoadMetros();
    LoadStations();
    LoadEdgesAndContexts();

    return 0;
}
