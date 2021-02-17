cmake_minimum_required(VERSION 3.19)
project(Wuhan-metro C)

set(CMAKE_C_STANDARD 11)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory (src SRC_LIST)

include_directories (include)

add_executable (main ${SRC_LIST} src/models.c include/models.h src/hash.c include/hash.h include/common.h src/common.c)