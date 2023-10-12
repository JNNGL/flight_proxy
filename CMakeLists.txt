cmake_minimum_required(VERSION 3.25)
project(flight_proxy C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES src/*.c)
add_executable(flight_proxy ${SOURCES})
target_link_libraries(flight_proxy pthread)
