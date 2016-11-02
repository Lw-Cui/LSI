find_package(SFML REQUIRED system window graphics network audio)

set(GUI_SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${GUI_SOURCE_FILES})
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()