CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(gtests)

ADD_EXECUTABLE(${PROJECT_NAME} gtest.cpp)
SET(CMAKE_CXX_STANDARD 11)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC src_code gtest gtest_main)