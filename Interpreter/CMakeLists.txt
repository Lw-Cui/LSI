include_directories(include)

set(LEXERS_LIB lexer CACHE INTERNAL "Name of lexers libraries")
set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")

set(EXCEPTION_FILES
        include/exception.h)

set(LEXERS_SOURCE_FILES
        ${EXCEPTION_FILES}
        parser/lexers.cpp include/lexers.h
        )
add_library(${LEXERS_LIB} ${LEXERS_SOURCE_FILES})

set(INTERPRETER_SOURCE_FILES
        ${LEXERS_SOURCE_FILES}
        parser/functionParser.cpp
        parser/keywordParser.cpp
        parser/basicParser.cpp
        evaluator/context.cpp include/context.h
        evaluator/AST.cpp include/AST.h
        evaluator/visitor.cpp include/visitor.h
        include/parser.h)
add_library(${INTERPRETER_LIB} ${INTERPRETER_SOURCE_FILES})

add_subdirectory(test)
