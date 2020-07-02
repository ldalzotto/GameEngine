cmake_minimum_required(VERSION 3.12)
project(Core VERSION 0.0.1)

file(GLOB_RECURSE CORE_PRIVATE 
	"./DataContainer/*.c",
	"./Error/*.c"
)
file(GLOB_RECURSE CORE_PUBLIC 
	"./DataContainer/*.h",
	"./Error/*.h"
)

add_library(Core STATIC ${CORE_PUBLIC} ${CORE_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(Core PUBLIC 
	"./"
)