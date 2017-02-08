set(BENCHMARK_FILES
        benchmark.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(BENCHMARK benchmark)
	add_executable(${BENCHMARK} ${BENCHMARK_FILES})
	target_link_libraries(${BENCHMARK} ${INTERPRETER_LIB} ${LEXERS_LIB})
endif (CMAKE_BUILD_TYPE STREQUAL "Release")
