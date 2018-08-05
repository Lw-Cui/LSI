set(CLI_SOURCE_FILES
        ../include/CLIbuiltinDrawAST.h CLIbuiltinDrawAST.cpp
        ../include/image.h image.cpp
        ../../external/easylogging/src/easylogging++.cc
        main.cpp)
add_executable(${PROJECT_NAME}_CLI ${CLI_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}_CLI ${INTERPRETER_LIB})
