//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_TRAFFICFLOWS_H
#define WUHAN_METRO_TRAFFICFLOWS_H

#include "models.h"

int InitTrafficFlowTable();


int AddSpecialCrowd(int typeID, int startHour, int startMin, int endHour, int endMin, double crowded);


int ChooseFlowType(const char* type);
#endif //WUHAN_METRO_TRAFFICFLOWS_H
