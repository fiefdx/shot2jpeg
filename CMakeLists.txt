cmake_minimum_required(VERSION 2.8)

project(libshot2jpeg_project)

add_library(shot2jpeg STATIC shot2jpeg.c)

add_executable(test_shot2jpeg_1 shot2jpeg.c test_shot2jpeg_1.c)
target_link_libraries(test_shot2jpeg_1 xcb xcb-image jpeg)

add_executable(test_shot2jpeg_2 shot2jpeg.c test_shot2jpeg_2.c)
target_link_libraries(test_shot2jpeg_2 xcb xcb-image jpeg)

add_executable(test_shot2jpeg_3 shot2jpeg.c test_shot2jpeg_3.c)
target_link_libraries(test_shot2jpeg_3 xcb xcb-image jpeg)