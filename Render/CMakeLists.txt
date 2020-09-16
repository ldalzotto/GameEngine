cmake_minimum_required(VERSION 3.12)
project(MyRender VERSION 0.0.1)

file(GLOB_RECURSE MYRENDER_SOURCE "./*.c")
file(GLOB_RECURSE MYRENDER_HEADER "./*.h")

add_compile_definitions(RENDER_PERFORMANCE_TIMER=1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(MyRender ${MYRENDER_HEADER} ${MYRENDER_SOURCE})

target_include_directories(MyRender PUBLIC "./")

target_link_libraries(MyRender PUBLIC Core_c)
target_link_libraries(MyRender PUBLIC Math)