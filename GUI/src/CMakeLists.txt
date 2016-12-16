set(TEXT_LIB TextWindow CACHE INTERNAL "text windows")
set(TEXT_SOURCE_FILES
        ../include/formatString.h formatString.cpp
        ../include/Controller.h Controller.cpp
        ../include/GUIbuiltinAST.h GUIbuiltinAST.cpp)
add_library(${TEXT_LIB} STATIC ${TEXT_SOURCE_FILES})

set(GUI_SOURCE_FILES
        main.cpp)
add_executable(${PROJECT_NAME} ${GUI_SOURCE_FILES})
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${INTERPRETER_LIB} ${TEXT_LIB} sfml-system sfml-window sfml-graphics)
