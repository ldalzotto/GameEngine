cmake_minimum_required(VERSION 3.12)
project(Core_c VERSION 0.0.1)

file(GLOB_RECURSE CORE_C_SOURCE "./*.c")
file(GLOB_RECURSE CORE_C_HEADER "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Core_c ${CORE_C_HEADER} ${CORE_C_SOURCE})

target_include_directories(Core_c PUBLIC "./")

