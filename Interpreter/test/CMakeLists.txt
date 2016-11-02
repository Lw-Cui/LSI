set(LEXERS_TEST_FILES lexersTest.cpp ${TEST_MAIN})
set(LEXERS_TEST lexersTest)
add_executable(${LEXERS_TEST} ${LEXERS_TEST_FILES})
target_link_libraries(${LEXERS_TEST} gtest)

set(PARSER_TEST_FILES parserTest.cpp ${TEST_MAIN})
set(PARSER_TEST parserTest)
add_executable(${PARSER_TEST} ${PARSER_TEST_FILES})
target_link_libraries(${PARSER_TEST} ${INTERPRETER_LIB} gtest)
