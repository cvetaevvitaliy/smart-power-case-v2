cmake_minimum_required(VERSION 3.7)

####################################################################################################
# Software version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

message(STATUS "Find lis3dh-lib ${CMAKE_CURRENT_SOURCE_DIR}")

set(SRC lis3dh_reg.c)
set(INC ./)

include_directories(${INC})
add_library(lib_lis3dh ${SRC} ${INC})