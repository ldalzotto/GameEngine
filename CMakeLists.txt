cmake_minimum_required(VERSION 3.12)
project(RootProject VERSION 0.0.1)

enable_language(C)
# set(BUILD_TYPE "")

add_compile_definitions(BUFFER_OVERFLOW_INTERNAL_CHECK=0)
add_compile_definitions(ASSETPATH_DEV=1)

add_subdirectory(ExternalDependencies)

add_subdirectory(Core_c)
add_subdirectory(ECS_C)
add_subdirectory(Math)
add_subdirectory(Render_Asset)
add_subdirectory(Render)
add_subdirectory(GameEngine)

add_subdirectory(Test)

add_subdirectory(Tools)