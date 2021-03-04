//
// Created by chen on 3/4/21.
//

#ifndef WUHAN_METRO_TIMES_H
#define WUHAN_METRO_TIMES_H
#include "models.h"

time_t Clock2Stamp(int hour, int min);

time_t Time2Stamp(Time* time);

// Returns TRUE
int Stamp2Time(time_t stp, Time* result);

int IsDuring(SpecialCrowded* spc, time_t time);

#endif //WUHAN_METRO_TIMES_H
