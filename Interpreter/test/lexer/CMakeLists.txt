set(LEXERS_TEST_FILES
        lexersTest.cpp)

set(LEXERS_TEST lexersTest)
add_executable(${LEXERS_TEST} ${LEXERS_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${LEXERS_TEST} ${GTEST_LIB} ${LEXERS_LIB})
