//
// Created by chen on 2/16/21.
//

#ifndef WUHAN_METRO_MODELS_H
#define WUHAN_METRO_MODELS_H

#define MAX_METRO_NAME_LEN 64
#define MAX_METRO_NUM_LEN 32

#define MAX_STATION_NAME_LEN 64
#define MAX_STATION_NUM_LEN 64
#define MAX_ALL_STATION_NUM 250

#define MAX_EDGE_NUM_LEN 64

#define STATION_STOP_TIME 60
#define TWO_STATIONS_RUN_TIME 120
#define TRANSFER_TIME 210

#define MAX_SPECIAL_CROWDEDS_LEN 6

#define TRAFFIC_FLOW_TYPE_NUM 15

#define TRANSFER_STATION_NUM_PER_METRO 30


typedef struct Metro Metro;
typedef struct Station Station;
typedef struct MetroContext MetroContext;
typedef struct Edge Edge;
typedef struct TrafficFlow TrafficFlow;
typedef struct MetroSystem MetroSystem;
typedef struct SpecialCrowded SpecialCrowded;
typedef struct TransferStationGraph TransferStationGraph;
typedef struct TransferStationNode TransferStationNode;
typedef struct Time Time;
typedef double time_t;

struct Metro {
    char            name[MAX_METRO_NAME_LEN];		// 地铁线路名称
    double 			length;                         //地铁物理长度
    int             capacity;
    int 			stationNum;	                    //站点数
    Station*		stations[MAX_STATION_NUM_LEN];	//站点信息
    Edge*			edges[MAX_EDGE_NUM_LEN];		//各边信息
    TrafficFlow* 	trafficFlow;	// 流量
};

struct Station {
    char 		    name[MAX_STATION_NAME_LEN]; //地铁线名字
    double 		    longitude;                  // 构造地图用的经纬度
    double          latitude;
    int			    metroNum;		            //所在线路条数
    MetroContext*   metroContexts[MAX_METRO_NUM_LEN];   // 铁道上下文

    double      costToHere; // the metrics to this node from a start node
    int         known;
    Station*    last_node;
    time_t      arrivalTime;
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
    double 			length;         // 物理长度
    double		    timeNeeded;     // 一次所需时间，单位s
};

struct MetroSystem {
    int             metroNum;   // 地铁线路总情况
    Metro*          metros[MAX_METRO_NUM_LEN];
    int             stationNum;
    Station*        stations[MAX_ALL_STATION_NUM];
};

// A state of a crowd: from hh:mm to hh:mm what the crowded(0~1) of a metro is
struct SpecialCrowded{
    double startStp;
    double endStp;
    double crowded; //(0,1)
};

struct TrafficFlow {
    int             specialNum;
    SpecialCrowded* specialCrowdeds[MAX_SPECIAL_CROWDEDS_LEN];
    double          defaultCrowded;
};


struct TransferStationGraph {
    int tsfStationNum;
    TransferStationNode* tsfStationNodes[MAX_STATION_NUM_LEN];
};


struct TransferStationNode {
    Station*    station;
    int         adjNum;
    TransferStationNode* adjNodes[TRANSFER_STATION_NUM_PER_METRO];
    double      edgeCosts[TRANSFER_STATION_NUM_PER_METRO]; // with the above attr

    // all below are loaded and used dynamically
    TransferStationNode*    lastNode;
    double      costToHere; // the metrics to this node from a start node
    int         known;
};



struct Time {
    int     hour;
    int     minute;
    time_t  second;
};
#endif //WUHAN_METRO_MODELS_H

