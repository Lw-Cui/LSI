include_directories(include)

set(TEST_MAIN
        ${CMAKE_SOURCE_DIR}/Interpreter/test/driver.cpp
        ${CMAKE_SOURCE_DIR}/external/easylogging/src/easylogging++.cc
        CACHE INTERNAL "Google test main function.")
set(GTEST_LIB gtest gmock CACHE INTERAL "Google test lib & mock lib")

set(TEST_FILES
        #core/exceptionTest.cpp
        core/basicParserTest.cpp
        core/functionParsingTest.cpp
        core/functionTest.cpp
        core/contextTest.cpp
        core/keywordTest.cpp
        core/builtinFunctionTest.cpp
        core/lexersTest.cpp
        stdlib/libParsingTest.cpp
        stdlib/BaselibParsingTest.cpp
        stdlib/FramelibParsingTest.cpp
        ../include/testMacro.h)

set(TEST InterpreterTest)
add_executable(${TEST} ${TEST_FILES} ${TEST_MAIN})
target_link_libraries(${TEST} ${INTERPRETER_LIB} ${GTEST_LIB})

# add_subdirectory(benchmark)
