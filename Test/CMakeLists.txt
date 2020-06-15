cmake_minimum_required(VERSION 3.12)
project(Test VERSION 0.0.1)

file(GLOB_RECURSE TEST_APP_PRIVATE "./*.cpp")
file(GLOB_RECURSE TEST_APP_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Test ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})

target_include_directories(Test PUBLIC "./")

target_link_libraries(Test PUBLIC GameEngine)
target_link_libraries(Test PUBLIC GameEngine_Editor)

copy_assimp_dll(${PROJECT_BINARY_DIR}/${BUILD_TYPE}/Debug)