//
// Created by chen on 3/1/21.
//

#ifndef WUHAN_METRO_METROCONTEXTS_H
#define WUHAN_METRO_METROCONTEXTS_H

#include "models.h"

// Returns a new MetroContext filled with basic information: metro & id of the station
//
// Memory: Allocates sizeof(MetroContext)
MetroContext* NewMetroContext(Metro* metro, int id);


// All contexts are already created when loading stations.
// Call this func after loading all stations
// No extra memory used.
void FillContext(Metro*);

#endif //WUHAN_METRO_METROCONTEXTS_H
