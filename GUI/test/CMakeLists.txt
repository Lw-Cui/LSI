set(GUI_TEST GUITest)
set(TEST_FILES
        textTest.cpp
        drawingBoardTest.cpp)

add_executable(${GUI_TEST} ${TEST_FILES} ${TEST_MAIN})
target_link_libraries(${GUI_TEST} ${GTEST_LIB} ${CONTROL_LIB} ${INTERPRETER_LIB} ${SFML_LIB})

