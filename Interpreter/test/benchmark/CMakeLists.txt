set(BENCHMARK_FILES
        benchmark.cpp)

set(BENCHMARK benchmark)
add_executable(${BENCHMARK} ${BENCHMARK_FILES})
target_link_libraries(${BENCHMARK} ${INTERPRETER_LIB} ${LEXERS_LIB})
