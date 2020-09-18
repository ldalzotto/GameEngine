cmake_minimum_required(VERSION 3.12)
project(Render_Asset VERSION 0.0.1)

file(GLOB_RECURSE RENDERASSET_SOURCE "./*.c")
file(GLOB_RECURSE RENDERASSET_HEADER "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Render_Asset ${RENDERASSET_HEADER} ${RENDERASSET_SOURCE})

target_include_directories(Render_Asset PUBLIC "./")

target_link_libraries(Render_Asset PUBLIC Math)