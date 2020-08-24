cmake_minimum_required(VERSION 3.12)
project(RootProject VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_TYPE "")

add_compile_definitions(RENDER_V2=1)

add_subdirectory(Core_c)
add_subdirectory(ECS_C)
add_subdirectory(Math)
add_subdirectory(Render)
add_subdirectory(GameEngine)

add_subdirectory(Test)