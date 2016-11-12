set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")
set(SOURCE_FILES parser.cpp context.cpp ../include/lexers.h ../include/parser.h ../include/context.h)
add_library(${INTERPRETER_LIB} STATIC ${SOURCE_FILES})
