//
// Created by chen on 3/4/21.
//

#include "times.h"
#include "common.h"

timestamp_t Time2Stamp(Time* time) {
    timestamp_t result = 0;
    result += time->hour * 3600;
    result += time->minute * 60;
    result += time->second;
    return result;
}


int Stamp2Time(timestamp_t stp, Time* result) {
    int int_stp = (int)stp;
    int hour = int_stp/3600;
    int_stp %= 3600;
    int min = int_stp/60;
    double second = stp - hour*3600 - min*60;
    result->hour = hour;
    result->minute = min;
    result->second = second;
    return TRUE;
}

int IsDuring(SpecialCrowded* spc, timestamp_t time) {
    if (time < spc->startStp || time >= spc->endStp) return FALSE;
    if (time > DAY_END_TIMESTAMP || time < 0) return FALSE;
    return TRUE;
}

timestamp_t Clock2Stamp(int hour, int min) {
    Time t = {hour, min, 0};
    return Time2Stamp(&t);
}