//
// Created by chen on 3/1/21.
//
#include "stdio.h"
#include "traverse.h"
#include "../models/models.h"
#include "../models/common.h"
extern MetroSystem* metroSystem;


int TraverseSystem() {
    int n = metroSystem->metroNum;
    for (int i = 0; i < n; i++) {
        Metro* metro = metroSystem->metros[i];
        printf("%s, 全长%fkm，有%d站点\n", metro->name, metro->length, metro->stationNum);
    }
    return TRUE;
}


void DFS_system(station) {

}