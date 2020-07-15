cmake_minimum_required(VERSION 3.12)
project(Core_cpp_test VERSION 0.0.1)

file(GLOB_RECURSE CORE_CPPTEST_SOURCE "./*.cpp")
file(GLOB_RECURSE CORE_CPPTEST_HEADER "./*.hpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Core_cpp_test ${CORE_CPPTEST_HEADER} ${CORE_CPPTEST_SOURCE})


target_include_directories(Core_cpp_test PUBLIC "./")

target_link_libraries(Core_cpp_test PUBLIC Core_cpp)
