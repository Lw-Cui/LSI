set(CORE_TEST_FILES
        exceptionTest.cpp
        basicParserTest.cpp
        functionParsingTest.cpp
        functionTest.cpp
        contextTest.cpp
        keywordTest.cpp
        builtinFunctionTest.cpp
        lexersTest.cpp
        ../../include/testMacro.h)

set(CORE_TEST coreTest)
add_executable(${CORE_TEST} ${CORE_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${CORE_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})

