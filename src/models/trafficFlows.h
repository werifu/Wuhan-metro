//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_TRAFFICFLOWS_H
#define WUHAN_METRO_TRAFFICFLOWS_H

#include "models.h"

// Returns TRUE
// Defines 4 types of traffic flow: work(0), shopping(1), entertainment(2), cities(3)
// Memory: Allocates sizeof(TrafficFlow)*4
int InitTrafficFlowTable();


// Memory: Allocates sizeof(SpecialCrowd)
int AddSpecialCrowd(int typeID, int startHour, int startMin, int endHour, int endMin, double crowded);


// Returns the id of the type name(string).
// For example: (ChooseFlowType("work") == 0) ==> true
int ChooseFlowType(const char* type);
#endif //WUHAN_METRO_TRAFFICFLOWS_H
