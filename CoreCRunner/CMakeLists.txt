cmake_minimum_required(VERSION 3.12)
project(CoreCRunner VERSION 0.0.1)

file(GLOB_RECURSE CoreCRunner_PRIVATE "./*.c")
file(GLOB_RECURSE CoreCRunner_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CoreCRunner ${CoreCRunner_PUBLIC} ${CoreCRunner_PRIVATE})

target_include_directories(CoreCRunner PUBLIC "./")

target_link_libraries(CoreCRunner PUBLIC CoreC)