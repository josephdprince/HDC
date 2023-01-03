CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

project(HDC)

add_subdirectory(src)
add_subdirectory(tests)

ADD_EXECUTABLE(${PROJECT_NAME} main.c)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} src_code)