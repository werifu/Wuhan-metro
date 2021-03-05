#include "models/models.h"
#include "models/metros.h"
#include "models/stations.h"
#include "models/edges.h"
#include "models/trafficFlows.h"
#include "utils/hash.h"
#include "stdio.h"
#include "test/traverse.h"
#include "handlers/paths.h"

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
    GetShortestPath(GetStation("华中科技大学"), GetStation("天河机场"), 60000);
    GetLeastTimePath(GetStation("华中科技大学"), GetStation("天河机场"), 60000);
    GetAvoidingCrowdPath(GetStation("华中科技大学"), GetStation("天河机场"), 60000);
    return 0;
}
