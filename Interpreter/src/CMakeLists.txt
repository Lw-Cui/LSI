set(LEXERS_LIB lexer CACHE INTERNAL "Name of lexers libraries")
set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")

set(LEXERS_SOURCE_FILES
        lexers.cpp
        ../include/lexers.h)
add_library(${LEXERS_LIB} SHARED ${LEXERS_SOURCE_FILES})

set(INTERPRETER_SOURCE_FILES
        functionParser.cpp
        keywordParser.cpp
        basicParser.cpp
        context.cpp
        AST.cpp
        ../include/parser.h
        ../include/context.h
        ../include/AST.h)
add_library(${INTERPRETER_LIB} SHARED ${INTERPRETER_SOURCE_FILES})
target_link_libraries(${INTERPRETER_LIB} ${LEXERS_LIB})
