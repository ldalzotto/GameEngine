cmake_minimum_required(VERSION 3.12)
project(GameEngine_Physics VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_PHYSICS_PRIVATE 
	"./World/*.cpp"
	"./CollisionEngine/*.cpp"
	"./World/*.c"
	"./CollisionEngine/*.c"
)

SET(GAME_ENGINE_PHYSICS_PRIVATE 
	${GAME_ENGINE_PHYSICS_PRIVATE} ${CMAKE_CURRENT_SOURCE_DIR}/Physics.cpp ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsInterface.cpp 
)

file(GLOB_RECURSE GAME_ENGINE_PHYSICS_PUBLIC 
	"./World/*.h"
	"./CollisionEngine/*.h"	
)

SET(GAME_ENGINE_PHYSICS_PUBLIC 
	${GAME_ENGINE_PHYSICS_PUBLIC} ${CMAKE_CURRENT_SOURCE_DIR}/Physics.h ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsInterface.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(GameEngine_Physics STATIC ${GAME_ENGINE_PHYSICS_PUBLIC} ${GAME_ENGINE_PHYSICS_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine_Physics PUBLIC "./")

target_link_libraries(GameEngine_Physics PUBLIC GameEngine_Common)