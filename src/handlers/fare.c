//
// Created by chen on 2/18/21.
//

#include "fare.h"

// unit of fare: 1 = 0.001yuan, 1yuan = 1000
int GetFare(double length) {
    int fare = 0;
    double tmp_len;
    // 50km = 50000m
    tmp_len = length-50;
    if (tmp_len > 0) {
        fare += (int)(tmp_len/20) + 1;
        fare += 9;
        return fare*1000;
    }
    tmp_len = length-40;
    if (tmp_len > 0) {
        fare += (int)(tmp_len/10) + 1;
        fare += 8;
        return fare*1000;
    }
    tmp_len = length-24;
    if (tmp_len > 0) {
        fare += (int)(tmp_len/8) + 1;
        fare += 6;
        return fare*1000;
    }
    tmp_len = length-12;
    if (tmp_len > 0) {
        fare += (int)(tmp_len/6) + 1;
        fare += 4;
        return fare*1000;
    }
    tmp_len = length-4;
    if (tmp_len > 0) {
        fare += (int)tmp_len/4 + 1;
        fare += 2;
        return fare*1000;
    }
    fare = 2;
    return fare*1000;
}

