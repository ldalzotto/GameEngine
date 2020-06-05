cmake_minimum_required(VERSION 3.12)
project(GameEngine_Editor VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_PRIVATE 
	"./*.cpp"
)
# SET(GAME_ENGINE_EDITOR_PRIVATE ${GAME_ENGINE_EDITOR_PRIVATE} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.cpp)

file(GLOB_RECURSE GAME_ENGINE_EDITOR_PUBLIC 
	"./*.h"
)
# SET(GAME_ENGINE_EDITOR_PUBLIC ${GAME_ENGINE_EDITOR_PUBLIC} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.h)

include_directories(
	"./"
)

add_library(GameEngine_Editor STATIC ${GAME_ENGINE_EDITOR_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine_Editor PUBLIC "./")

target_link_libraries(GameEngine_Editor PUBLIC GameEngine)