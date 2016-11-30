set(TEXT_TEST TextTest)
set(TEXT_TEST_FILES
        textTest.cpp)

add_executable(${TEXT_TEST} ${TEXT_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${TEXT_TEST} gtest ${TEXT_LIB} ${INTERPRETER_LIB})
