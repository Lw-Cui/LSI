set(LEXERS_TEST_FILES lexersTest.cpp)
set(LEXERS_TEST lexersTest)
add_executable(${LEXERS_TEST} ${LEXERS_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${LEXERS_TEST} gtest ${LEXERS_LIB})

set(PARSER_TEST_FILES
        basicParserTest.cpp
        functionParsingTest.cpp
        keywordTest.cpp
        librariesParsingTest.cpp
        builtinFunctionTest.cpp)

set(PARSER_TEST parserTest)
add_executable(${PARSER_TEST} ${PARSER_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${PARSER_TEST} ${INTERPRETER_LIB} gtest)

set(INTERPRETER_TEST InterpreterTest)
add_executable(${INTERPRETER_TEST} ${LEXERS_TEST_FILES} ${PARSER_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${INTERPRETER_TEST} ${INTERPRETER_LIB} gtest)

include(CodeCoverage)
setup_target_for_coverage(CoverageTest ${INTERPRETER_TEST} coverage)
