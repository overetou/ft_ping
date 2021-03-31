cmake_minimum_required(VERSION 3.10)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(includes)

add_executable(ft_ping src/main.c includes/ft_ping.h src/arguments_processing.c src/str_manip.c src/network_part.c)