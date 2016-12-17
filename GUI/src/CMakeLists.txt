set(CONTROL_LIB Contorller CACHE INTERNAL "text windows")
set(CONTROL_SOURCE_FILES
        ../include/formatString.h formatString.cpp
        ../include/Controller.h Controller.cpp
        ../include/GUIbuiltinAST.h GUIbuiltinAST.cpp)
add_library(${CONTROL_LIB} SHARED ${CONTROL_SOURCE_FILES})
target_link_libraries(${CONTROL_LIB} ${INTERPRETER_LIB} ${SFML_LIB})

set(GUI_SOURCE_FILES
        main.cpp)
add_executable(${PROJECT_NAME} ${GUI_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONTROL_LIB})
