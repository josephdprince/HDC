PROJECT(source_code)

ADD_LIBRARY(src_code HDC.c HDC.h)

TARGET_LINK_LIBRARIES(src_code PUBLIC m)