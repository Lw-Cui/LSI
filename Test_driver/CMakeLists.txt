set(TEST_MAIN
        ${CMAKE_SOURCE_DIR}/Test_driver/main.cpp
        ${CMAKE_SOURCE_DIR}/external/easylogging/src/easylogging++.cc
        CACHE INTERNAL "Google test main function.")
set(GTEST_LIB gtest gmock CACHE INTERAL "Google test lib & mock lib")
