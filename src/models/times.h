//
// Created by chen on 3/4/21.
//

#ifndef WUHAN_METRO_TIMES_H
#define WUHAN_METRO_TIMES_H
#include "models.h"

timestamp_t Clock2Stamp(int hour, int min);

timestamp_t Time2Stamp(Time* time);

// Returns TRUE
int Stamp2Time(timestamp_t stp, Time* result);

int IsDuring(SpecialCrowded* spc, timestamp_t time);

#endif //WUHAN_METRO_TIMES_H
