cmake_minimum_required(VERSION 3.6)
project(wolf3d)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lmlx -framework OpenGL -framework AppKit")
set(SOURCE_FILES
    main.c wolf.h colour.c print.c additional.c map.c key_funk.c rawcast_calc.c)

add_executable(wolf3d ${SOURCE_FILES})
#[[target_link_libraries(FRACTOL ${CMAKE_SOURCE_DIR}/libft/libft.a)]]


