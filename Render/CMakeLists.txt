cmake_minimum_required(VERSION 3.12)
project(MyRender VERSION 0.0.1)

file(GLOB_RECURSE MYRENDER_SOURCE "./*.cpp")
file(GLOB_RECURSE MYRENDER_HEADER "./*.h" "./*.hpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(MyRender ${MYRENDER_HEADER} ${MYRENDER_SOURCE} "Cull/BackfaceCulling.cpp")

target_include_directories(MyRender PUBLIC "./")

target_link_libraries(MyRender PUBLIC Core_cpp)
target_link_libraries(MyRender PUBLIC Math)