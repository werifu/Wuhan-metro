//
// Created by chen on 2/16/21.
//

#ifndef WUHAN_METRO_MODELS_H
#define WUHAN_METRO_MODELS_H

#define MAX_METRO_NAME_LEN 64
#define MAX_METRO_NUM_LEN 32

#define MAX_STATION_NAME_LEN 64
#define MAX_STATION_NUM_LEN 64

#define MAX_EDGE_NUM_LEN 64

#define STATION_STOP_TIME 60
#define TWO_STATIONS_RUN_TIME 120

typedef struct Metro Metro;
typedef struct Station Station;
typedef struct MetroContext MetroContext;
typedef struct Edge Edge;
typedef struct TrafficFlow TrafficFlow;
typedef struct MetroSystem MetroSystem;

struct Metro {
    char            name[MAX_METRO_NAME_LEN];		// 地铁线路名称
    int 			length;                         //地铁物理长度
    int             capacity;
    int 			stationNum;	                    //站点数
    Station*		stations[MAX_STATION_NUM_LEN];	//站点信息
    Edge*			edges[MAX_EDGE_NUM_LEN];		//各边信息
};

struct Station {
    char 		    name[MAX_STATION_NAME_LEN]; //地铁线名字
    int 		    posX;                       // 构造地图用的横坐标
    int             posY;
    int			    metroNum;		            //所在线路条数
    MetroContext*   metroContexts[MAX_METRO_NUM_LEN];   // 铁道上下文
};

struct MetroContext {
    Metro*		metro;		//当前线路
    Station*	nextStation;//当前线路的下一序号站，没有则为nullptr
    Station*	lastStation;
    Edge*		nextEdge;	//以当前站点为起点的边
    Edge*		lastEdge;	//以当前站点为终点的边
    int 		id;			//站点在该线路序号
};

struct Edge {
    Station* 		startStation;	// 起点站（起->终表明了方向）
    Station* 		endStation;
    Metro* 			metro;			// 所在地铁线
    TrafficFlow* 	trafficFlow;	// 流量
    int 			length;         // 物理长度
    int				timeNeeded;     // 一次所需时间，单位s
};

struct MetroSystem {
    int             metroNum;   // 地铁线路总情况
    Metro*          metros[MAX_METRO_NUM_LEN];
};


// Inits the main Metro System. Returns the status(TRUE, FALSE, ERROR or PANIC) of the operation. Loads the data into the extern variable metroSystem defined in models.c
//
// Memory: Allocates sizeof(MetroSystem)
int InitMetroSystem();

// Returns a new Metro filled with name & length. No stations and edges in it.
//
// Params: 1.(char*) the name of the metro; 2.(int) the physical length of the metro. 3.(int)passenger capacity of the metro
// Memory: Allocates sizeof(Metro)
Metro* NewMetro(char* name, int length, int capacity);

// Returns a new Station filled with name & length. No MetroContext in it. Returns NULL if failing.
//
// Params: 1.(char*) the name of the station;
//         2.(Metro*) the metro the station is on;
//         3&4.(int) posX/posY is the position of the station on a map;
//         5.(int) id of the station on the current metro.
// Memory: Allocates sizeof(Station)+sizeof(StationContext) when there is no this station. Otherwise allocates sizeof(MetroContext).
Station* NewStation(char* name, Metro* metro, int posX, int posY, int id);

// Returns a new Edge filled with all properties.
//
// NULL: pointers variable can be NULL
// Memory: Allocates sizeof(Edge)
Edge* NewEdge(Station* startStation, Station* endStation, Metro* metro, TrafficFlow* trafficFlow, int length, int timeNeeded);

// Returns a new MetroContext filled with basic information: metro & id of the station
//
// Memory: Allocates sizeof(MetroContext)
MetroContext* NewMetroContext(Metro* metro, int id);


// Inits the hash table storing the all stations
// Returns TRUE if ok, PANIC if cannot allocate
// Memory: sizeof(StationTable*)*STATION_TABLE_LEN
int InitStationTable();

// Inits the hash table storing the all stations
// Returns TRUE if ok, PANIC if cannot allocate
// Memory: sizeof(MetroTable*)*METRO_TABLE_LEN
int InitMetroTable();

// Loads basic info of metros from "data/metros.csv"
// Format of csv: metro_name,capacity,length,station_num
// Returns TRUE if ok, returns ERROR if cannot open the file
// Memory: Allocates sizeof(Metro) * Number of metros in the .csv
int LoadMetros();

// Loads basic info of metros from "data/stations.csv"
// Format of csv: station_name,pos_x,pos_y,metro_name,id
// Returns TRUE if ok, returns ERROR if cannot open the file
// Memory: Allocates sizeof(Station) * Number of individual stations in the .csv (not duplicate)
int LoadStations();

// Called after LoadMetro() and LoadStations()
// Along all metros fills the edges and contexts of the stations.
// Returns TRUE if ok
// Loads metro data from "data/metros.csv"
int LoadEdgesAndContexts();

// Along a metro.
void FillContext(Metro*);
#endif //WUHAN_METRO_MODELS_H

