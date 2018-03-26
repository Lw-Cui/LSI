cmake_minimum_required(VERSION 3.2)
project(LSI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Coverage")

add_subdirectory(external/easylogging)
include_directories(external/easylogging/src)

add_subdirectory(external/cxxopts)
include_directories(external/cxxopts/include)

include_directories(external/CImg)

add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_FILE}/include)
include_directories(${gmock_SOURCE_FILE}/include)

add_subdirectory(Test_driver)
add_subdirectory(Interpreter)
add_subdirectory(Interface/CLI)

# GUI is out of date
# add_subdirectory(Interface/GUI)
