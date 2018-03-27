set(LISP_LIB_TEST_FILES
        libParsingTest.cpp
        BaselibParsingTest.cpp
        FramelibParsingTest.cpp)

set(LISP_LIB_TEST stdlibTest)
add_executable(${LISP_LIB_TEST} ${LISP_LIB_TEST_FILES} ${TEST_MAIN})
target_link_libraries(${LISP_LIB_TEST} ${INTERPRETER_LIB} ${GTEST_LIB})
