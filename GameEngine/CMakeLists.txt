cmake_minimum_required(VERSION 3.12)
project(GameEngine VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_PRIVATE 
	"./ECSEngine/*.c"
	"./GameLoop/*.c"
	"./Input/*.c"
	"./Editor/*cpp" "./Editor/*c"
)
SET(GAME_ENGINE_PRIVATE ${GAME_ENGINE_PRIVATE} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.c ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplicationInterface.c)

file(GLOB_RECURSE GAME_ENGINE_PUBLIC 
	"./ECSEngine/*.h"
	"./GameLoop/*.h"
	"./Input/*.h"
	"./EngineSequencers/*.h"
	"./Editor/*h"
)
SET(GAME_ENGINE_PUBLIC ${GAME_ENGINE_PUBLIC} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplicationConfiguration.h ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.h ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplicationInterface.h)

include_directories(
	"./ECS"
	"./ECS_Impl"
	"./GameLoop"
	"./Input"
)

add_library(GameEngine STATIC ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine PUBLIC "./")

add_subdirectory(Common)
add_subdirectory(Physics)

target_link_libraries(GameEngine PUBLIC GameEngine_Common)
target_link_libraries(GameEngine PUBLIC Math)
target_link_libraries(GameEngine PUBLIC ECS_C)
target_link_libraries(GameEngine PUBLIC MyRender)
target_link_libraries(GameEngine PUBLIC GameEngine_Physics)