cmake_minimum_required(VERSION 3.12)
project(ECS_C VERSION 0.0.1)

file(GLOB_RECURSE ECS_C_SOURCE "./*.c")
file(GLOB_RECURSE ECS_C_HEADER "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(ECS_C ${ECS_C_HEADER} ${ECS_C_SOURCE})

target_include_directories(ECS_C PUBLIC "./")

target_link_libraries(ECS_C PUBLIC Core_c)