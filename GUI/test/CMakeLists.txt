set(TEXT_TEST_FILES
        textTest.cpp)

set(TEXT_TEST TextTest)
add_executable(${TEXT_TEST} ${TEXT_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${TEXT_TEST} gtest)
