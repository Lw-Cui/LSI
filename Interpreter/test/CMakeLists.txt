set(LEXERS_TEST_FILES
        lexersTest.cpp)

set(LEXERS_TEST lexersTest)
add_executable(${LEXERS_TEST} ${LEXERS_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${LEXERS_TEST} ${GTEST_LIB} ${LEXERS_LIB})

set(PARSER_TEST_FILES
        basicParserTest.cpp
        functionParsingTest.cpp
        keywordTest.cpp
        builtinFunctionTest.cpp)

set(PARSER_TEST parserTest)
add_executable(${PARSER_TEST} ${PARSER_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${PARSER_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})

set(LISP_LIB_TEST_FILES
        BaselibParsingTest.cpp
        FramelibParsingTest.cpp)

set(LISP_LIB_TEST lispLibTest)
add_executable(${LISP_LIB_TEST} ${LISP_LIB_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${LISP_LIB_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})

set(INTERPRETER_TEST InterpreterTest)
add_executable(${INTERPRETER_TEST} ${LEXERS_TEST_FILES} ${PARSER_TEST_FILES} ${LISP_LIB_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${INTERPRETER_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})
