cmake_minimum_required(VERSION 3.2)
project(GI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmakeModules")

include_directories(external/easylogging)
add_subdirectory(test)
add_subdirectory(external/googletest)
add_subdirectory(Interpreter)
add_subdirectory(GUI)

