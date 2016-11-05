cmake_minimum_required(VERSION 2.8)
project(GI)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakeModules")

if (DEFINED ENV{CoverageTest})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
endif ()

include_directories(external/easylogging)
add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_FILE}/include)
include_directories(${gmock_SOURCE_FILE}/include)

add_subdirectory(test)
add_subdirectory(Interpreter)
add_subdirectory(GUI)

