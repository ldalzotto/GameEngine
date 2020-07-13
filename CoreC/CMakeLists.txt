cmake_minimum_required(VERSION 3.12)
project(CoreC VERSION 0.0.1)

file(GLOB_RECURSE CORE_SOURCE "./*.c" "./*.cpp")
file(GLOB_RECURSE CORE_HEADER "./*.h" "./*.hpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(CoreC ${CORE_HEADER} ${CORE_SOURCE})

target_include_directories(CoreC PUBLIC "./")

