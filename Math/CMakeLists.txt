cmake_minimum_required(VERSION 3.12)
project(Math VERSION 0.0.1)

file(GLOB_RECURSE MATH_SOURCE "./*.c")
file(GLOB_RECURSE MATH_HEADER "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Math ${MATH_HEADER} ${MATH_SOURCE})

target_include_directories(Math PUBLIC "./")

target_link_libraries(Math PUBLIC Core_c)