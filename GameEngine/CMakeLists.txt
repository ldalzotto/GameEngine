cmake_minimum_required(VERSION 3.12)
project(GameEngine VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_PRIVATE "./*.cpp")
file(GLOB_RECURSE GAME_ENGINE_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(GameEngine STATIC ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine PUBLIC "./")

target_link_libraries(GameEngine PUBLIC glm)
target_link_libraries(GameEngine PUBLIC GLFW)
target_link_libraries(GameEngine PUBLIC spdlog)
target_link_libraries(GameEngine PUBLIC STB_IMAGE)