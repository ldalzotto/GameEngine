cmake_minimum_required(VERSION 3.12)
project(GameEngine VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_PRIVATE 
	"./ECS/*.cpp"
	"./ECS_Impl/*.cpp"
	"./GameLoop/*.cpp"
	"./Input/*.cpp"
	"./EngineSequencers/*.cpp"
	"RenderHook.cpp"
)
SET(GAME_ENGINE_PRIVATE ${GAME_ENGINE_PRIVATE} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.cpp ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplicationInterface.cpp)

file(GLOB_RECURSE GAME_ENGINE_PUBLIC 
	"./ECS/*.h"
	"./ECS_Impl/*.h"
	"./GameLoop/*.h"
	"./Input/*.h"
	"./EngineSequencers/*.h"
	"RenderHook.h"
)
SET(GAME_ENGINE_PUBLIC ${GAME_ENGINE_PUBLIC} ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplication.h ${CMAKE_CURRENT_SOURCE_DIR}/GameEngineApplicationInterface.h)

include_directories(
	"./ECS"
	"./ECS_Impl"
	"./GameLoop"
	"./Input"
	"./EngineSequencers"
)

add_library(GameEngine STATIC ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine PUBLIC "./")

add_subdirectory(Common)
add_subdirectory(Render)
add_subdirectory(Physics)

target_link_libraries(GameEngine PUBLIC GLFW)
target_link_libraries(GameEngine PUBLIC GameEngine_Common)
target_link_libraries(GameEngine PUBLIC GameEngine_Render)
target_link_libraries(GameEngine PUBLIC GameEngine_Physics)