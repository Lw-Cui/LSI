find_package(SFML REQUIRED system window graphics network audio)

include_directories(../../Interpreter/include)

set(GUI_SOURCE_FILES
        ../include/textWindow.h textWindow.cpp
        main.cpp)
add_executable(${PROJECT_NAME} ${GUI_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${INTERPRETER_LIB})

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif ()