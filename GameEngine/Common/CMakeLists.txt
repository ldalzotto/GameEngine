cmake_minimum_required(VERSION 3.12)
project(GameEngine_Common VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_COMMON_PRIVATE 
	"./Colors/*.c"
)
file(GLOB_RECURSE GAME_ENGINE_COMMON_PUBLIC 
	"./Colors/*.h"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(GameEngine_Common STATIC ${GAME_ENGINE_COMMON_PUBLIC} ${GAME_ENGINE_COMMON_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine_Common PUBLIC "./")

target_link_libraries(GameEngine_Common Core_c)
target_link_libraries(GameEngine_Common Math)