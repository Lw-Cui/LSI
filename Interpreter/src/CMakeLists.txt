set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")
set(SOURCE_FILES parser.cpp ../include/lexers.h ../include/parser.h)
add_library(${INTERPRETER_LIB} STATIC ${SOURCE_FILES})
