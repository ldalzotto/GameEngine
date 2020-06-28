cmake_minimum_required(VERSION 3.12)
project(Documentation VERSION 0.0.1)

file(GLOB_RECURSE DOCUMENTATION_PRIVATE "./*.cpp")
file(GLOB_RECURSE DOCUMENTATION_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Documentation ${DOCUMENTATION_PUBLIC} ${DOCUMENTATION_PRIVATE})

target_include_directories(Documentation PUBLIC "./")

target_link_libraries(Documentation PUBLIC GameEngine)