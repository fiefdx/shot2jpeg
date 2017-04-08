cmake_minimum_required(VERSION 2.8)

project(libshot2jpeg_project)

add_library(shot2jpeg STATIC shot2jpeg.c)

add_executable(test_shot2jpeg shot2jpeg.c test_shot2jpeg.c)

target_link_libraries(test_shot2jpeg xcb xcb-image jpeg)