cmake_minimum_required(VERSION 3.17)
project(ft_ping C)

set(CMAKE_C_STANDARD 99)

include_directories(includes)

add_executable(ft_ping src/main.c includes/ft_ping.h)