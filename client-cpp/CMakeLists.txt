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

project(client-cpp C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")

find_library(LAMBDA_RMI_API lambda-rmi-api ../api/target/cmake NO_DEFAULT_PATH)
find_library(ICE NAMES Ice PATHS $ENV{ICE_HOME}/lib $ENV{ICE_HOME}/lib64 NO_DEFAULT_PATH)
find_library(ICE_UTIL NAMES IceUtil PATHS $ENV{ICE_HOME}/lib $ENV{ICE_HOME}/lib64 NO_DEFAULT_PATH)

include_directories(SYSTEM $ENV{ICE_HOME}/include)
include_directories(SYSTEM ../api/target/cmake/generated/slice2cpp)

add_executable(client-cpp
    src/main/cpp/main.cpp
    src/main/cpp/SyncProcessor.cpp
    src/main/cpp/AsyncProcessor.cpp
    src/main/cpp/LambdaProcessor.cpp)
target_link_libraries(client-cpp
    ${LAMBDA_RMI_API}
    ${ICE}
    ${ICE_UTIL}
    pthread)
