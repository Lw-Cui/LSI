set(LEXERS_LIB lexer CACHE INTERNAL "Name of lexers libraries")
set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")

set(EXCEPTION_FILES
        ../include/exception.h)

set(LEXERS_SOURCE_FILES
        ${EXCEPTION_FILES}
        lexers.cpp
        ../include/lexers.h
        )
add_library(${LEXERS_LIB} ${LEXERS_SOURCE_FILES})

set(INTERPRETER_SOURCE_FILES
        ${LEXERS_SOURCE_FILES}
        functionParser.cpp
        keywordParser.cpp
        basicParser.cpp
        context.cpp
        AST.cpp
        ../include/parser.h
        ../include/context.h
        ../include/AST.h)
add_library(${INTERPRETER_LIB} ${INTERPRETER_SOURCE_FILES})
