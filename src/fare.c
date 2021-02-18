//
// Created by chen on 2/18/21.
//

#include "fare.h"

// make optimization
int GetFare(int length) {
    int fare = 0;
    int tmp_len;
    // 50km = 50000m
    tmp_len = length-50000;
    if (tmp_len > 0) {
        fare += tmp_len/200 + 100;
        fare += 900;
        return fare;
    }
    tmp_len = length-40000;
    if (tmp_len > 0) {
        fare += tmp_len/100 + 100;
        fare += 800;
        return fare;
    }
    tmp_len = length-24000;
    if (tmp_len > 0) {
        fare += tmp_len/80 + 100;
        fare += 600;
        return fare;
    }
    tmp_len = length-12000;
    if (tmp_len > 0) {
        fare += tmp_len/60 + 100;
        fare += 400;
        return fare;
    }
    tmp_len = length-4000;
    if (tmp_len > 0) {
        fare += tmp_len/40 + 100;
        fare += 200;
        return fare;
    }
    fare = 200;
    return fare;
}

