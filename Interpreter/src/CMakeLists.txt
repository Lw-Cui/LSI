set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")
set(SOURCE_FILES
        parser.cpp context.cpp AST.cpp lexers.cpp
        ../include/parser.h ../include/context.h ../include/AST.h ../include/lexers.h)
add_library(${INTERPRETER_LIB} STATIC ${SOURCE_FILES})
