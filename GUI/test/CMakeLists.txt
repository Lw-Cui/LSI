set(TEXT_TEST TextTest)
set(TEXT_TEST_FILES
        textTest.cpp)

find_package(SFML REQUIRED system window graphics network audio)
add_executable(${TEXT_TEST} ${TEXT_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${TEXT_TEST} gtest ${TEXT_LIB} ${SFML_LIBRARIES} ${INTERPRETER_LIB})
