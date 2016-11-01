set(INTERPRETER_LIB interpreter CACHE INTERNAL "Name of interpreter libraries")
set(SOURCE_FILES lexers.cpp ../include/lexers.h)
add_library(${INTERPRETER_LIB} STATIC ${SOURCE_FILES})
