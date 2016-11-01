set(LEXERS_TEST_FILES lexersTest.cpp ${TEST_MAIN})
set(LEXERS_TEST lexersTest)
add_executable(${LEXERS_TEST} ${LEXERS_TEST_FILES})
target_link_libraries(${LEXERS_TEST} ${INTERPRETER_LIB} gtest)