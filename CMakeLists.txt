cmake_minimum_required(VERSION 3.6)
project(wolf3d)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lXext -lX11 -lm")
set(SOURCE_FILES
    main.c  colour.c print.c additional.c)

add_executable(wolf3d ${SOURCE_FILES})
target_link_libraries(wolf3d ${CMAKE_SOURCE_DIR}/../minilibx/libmlx_Linux.a)


