set(CONTROL_LIB Contorller CACHE INTERNAL "text windows")
set(CONTROL_SOURCE_FILES
        ../include/formatString.h formatString.cpp
        ../include/Controller.h Controller.cpp
        ../include/GUIbuiltinDrawAST.h GUIbuiltinDrawAST.cpp)
add_library(${CONTROL_LIB} ${CONTROL_SOURCE_FILES})

set(GUI_SOURCE_FILES
        ../../../external/easylogging/src/easylogging++.cc
        main.cpp)
add_executable(${PROJECT_NAME}_GUI ${GUI_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_GUI ${CONTROL_LIB} ${INTERPRETER_LIB} ${SFML_LIB})
