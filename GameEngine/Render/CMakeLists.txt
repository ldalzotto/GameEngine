cmake_minimum_required(VERSION 3.12)
project(GameEngine_Render VERSION 0.0.1)

file(GLOB_RECURSE GAME_ENGINE_RENDER_PRIVATE 
	"./Gizmo/*.cpp"
	"./Includes/*.cpp"
	"./LoopStep/*.cpp"
	"./Materials/*.cpp"
	"./Mesh/*.cpp"
	"./Resources/*.cpp"
	"./Shader/*.cpp"
	"./Texture/*.cpp"
	"./VulkanObjects/*.cpp"
)
SET(GAME_ENGINE_RENDER_PRIVATE 
	${GAME_ENGINE_RENDER_PRIVATE} ${CMAKE_CURRENT_SOURCE_DIR}/Render.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/RenderInterface.cpp
)

file(GLOB_RECURSE GAME_ENGINE_RENDER_PUBLIC 
	"./Gizmo/*.h"
	"./Includes/*.h"
	"./LoopStep/*.h"
	"./Materials/*.h"
	"./Mesh/*.h"
	"./Resources/*.h"
	"./Shader/*.h"
	"./Texture/*.h"
	"./VulkanObjects/*.h"
)
SET(GAME_ENGINE_RENDER_PUBLIC 
	${GAME_ENGINE_RENDER_PUBLIC} ${CMAKE_CURRENT_SOURCE_DIR}/Render.h 
	${CMAKE_CURRENT_SOURCE_DIR}/RenderInterface.h
	${CMAKE_CURRENT_SOURCE_DIR}/RenderHook.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(GameEngine_Render STATIC ${GAME_ENGINE_RENDER_PUBLIC} ${GAME_ENGINE_RENDER_PRIVATE})
# add_executable(GameEngine ${GAME_ENGINE_PUBLIC} ${GAME_ENGINE_PRIVATE})

target_include_directories(GameEngine_Render PUBLIC "./")

target_link_libraries(GameEngine_Render PUBLIC GameEngine_Common)
target_link_libraries(GameEngine_Render PUBLIC GLFW)
target_link_libraries(GameEngine_Render PUBLIC STB_IMAGE)
target_link_libraries(GameEngine_Render PUBLIC ASSIMP)
target_link_libraries(GameEngine_Render PUBLIC IMGUI)