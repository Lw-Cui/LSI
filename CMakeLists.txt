cmake_minimum_required(VERSION 3.2)
project(GI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif (NOT CMAKE_BUILD_TYPE)
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")

add_subdirectory(external/easylogging)
include_directories(external/easylogging/src)

add_subdirectory(external/cxxopts)
include_directories(external/cxxopts/include)

include_directories(external/CImg)

add_subdirectory(external/googletest)
include_directories(${gtest_SOURCE_FILE}/include)
include_directories(${gmock_SOURCE_FILE}/include)

add_subdirectory(test)
add_subdirectory(Interpreter)
add_subdirectory(GUI)
add_subdirectory(CLI)
