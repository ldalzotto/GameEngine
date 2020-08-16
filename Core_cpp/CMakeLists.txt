cmake_minimum_required(VERSION 3.12)
project(Core_cpp VERSION 0.0.1)

file(GLOB_RECURSE CORE_CPP_SOURCE "./*.c" "./*.cpp")
file(GLOB_RECURSE CORE_CPP_HEADER "./*.h" "./*.hpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Core_cpp ${CORE_CPP_HEADER} ${CORE_CPP_SOURCE})

target_include_directories(Core_cpp PUBLIC "./")

