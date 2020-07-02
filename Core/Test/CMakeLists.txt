cmake_minimum_required(VERSION 3.12)
project(CoreTest VERSION 0.0.1)

file(GLOB_RECURSE CoreTest_PRIVATE "./*.cpp")
file(GLOB_RECURSE CoreTest_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CoreTest ${CoreTest_PUBLIC} ${CoreTest_PRIVATE})

target_include_directories(CoreTest PUBLIC "./")

target_link_libraries(CoreTest PUBLIC Core)