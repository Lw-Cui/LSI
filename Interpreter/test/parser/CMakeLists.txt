set(PARSER_TEST_FILES
        exceptionTest.cpp
        basicParserTest.cpp
        functionParsingTest.cpp
        keywordTest.cpp
        builtinFunctionTest.cpp
        ../../include/testMacro.h)

set(PARSER_TEST parserTest)
add_executable(${PARSER_TEST} ${PARSER_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${PARSER_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})

