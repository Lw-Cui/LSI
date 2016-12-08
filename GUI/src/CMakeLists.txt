set(TEXT_LIB TextWindow CACHE INTERNAL "text windows")
set(TEXT_SOURCE_FILES
        ../include/formatString.h formatString.cpp)
add_library(${TEXT_LIB} STATIC ${TEXT_SOURCE_FILES})

if (NOT DEFINED ENV{DO_NOT_BUILD_GUI})
    set(GUI_SOURCE_FILES
            ../include/Controller.h Controller.cpp
            ../include/GUIbuiltinFunc.h GUIbuiltinFunc.cpp
            main.cpp)
    add_executable(${PROJECT_NAME} ${GUI_SOURCE_FILES})
    find_package(SFML REQUIRED system window graphics network audio)
    if (SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} ${INTERPRETER_LIB} ${TEXT_LIB} ${SFML_LIBRARIES})
    endif ()
endif ()