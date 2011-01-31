#
# lambda-rmi - a demonstration of using lambda funcitons for RMI
#
# Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
#
# This program is free software, distributed under the terms of
# the BSD License. See the LICENSE.txt file at the top of the
# source tree.
#
cmake_minimum_required(VERSION 2.8)

set(SLICE2CPP slice2cpp)
set(SLICE2CPP_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/slice2cpp)

project(lambda-rmi-api)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

file(MAKE_DIRECTORY ${SLICE2CPP_DIR})
include_directories(${SLICE2CPP_DIR})

include_directories($ENV{ICE_HOME}/include)

add_custom_command(
    OUTPUT ${SLICE2CPP_DIR}/Sensor.cpp ${SLICE2CPP_DIR}/Sensor.h
    COMMAND ${SLICE2CPP} ${SLICE2CPPFLAGS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/slice/Sensor.ice
    --output-dir ${SLICE2CPP_DIR}
    COMMENT "slice2cpp Sensor.ice"
    DEPENDS src/main/slice/Sensor.ice)

add_library(lambda-rmi-api STATIC ${SLICE2CPP_DIR}/Sensor.cpp)
