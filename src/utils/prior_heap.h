//
// Created by chen on 3/2/21.
//

#ifndef WUHAN_METRO_PRIOR_HEAP_H
#define WUHAN_METRO_PRIOR_HEAP_H
#include "../models/models.h"


int BuildHeap();

TransferStationNode* GetHeapTop();

int HeapInsert(TransferStationNode node, double metrics);

#endif //WUHAN_METRO_PRIOR_HEAP_H
