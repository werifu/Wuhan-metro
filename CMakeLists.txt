cmake_minimum_required(VERSION 3.19)
project(Wuhan-metro C)

set(CMAKE_C_STANDARD 11)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory (src SRC_LIST)

include_directories (include)

add_executable (main ${SRC_LIST} src/models/models.c src/models/models.h src/utils/hash.c src/utils/hash.h src/models/common.h src/models/common.c src/handlers/fare.c src/handlers/fare.h src/handlers/metro_time.c src/handlers/metro_time.h src/handlers/paths.c src/handlers/paths.h src/test/test.c src/test/test.h src/models/metros.c src/models/metros.h src/models/stations.c src/models/stations.h src/models/edges.c src/models/edges.h src/models/trafficFlows.c src/models/trafficFlows.h src/models/metroContexts.c src/models/metroContexts.h src/handlers/handler_models.c src/handlers/handler_models.h src/test/traverse.c src/test/traverse.h)