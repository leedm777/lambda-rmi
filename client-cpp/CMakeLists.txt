cmake_minimum_required(VERSION 2.8)

project(client-cpp C CXX)

find_library(LAMBDA_RMI_API lambda-rmi-api ../api/target/cmake NO_DEFAULT_PATH)
find_library(ICE NAMES Ice PATHS $ENV{ICE_HOME}/lib $ENV{ICE_HOME}/lib64 NO_DEFAULT_PATH)
find_library(ICE_UTIL NAMES IceUtil PATHS $ENV{ICE_HOME}/lib $ENV{ICE_HOME}/lib64 NO_DEFAULT_PATH)

include_directories($ENV{ICE_HOME}/include)
include_directories(../api/target/cmake/generated/slice2cpp)

add_executable(client-cpp
    src/main/cpp/main.cpp
    src/main/cpp/SyncProcessor.cpp
    src/main/cpp/AsyncProcessor.cpp)
target_link_libraries(client-cpp
    ${LAMBDA_RMI_API}
    ${ICE}
    ${ICE_UTIL})
