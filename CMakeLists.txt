cmake_minimum_required(VERSION 3.12)
project(RootProject VERSION 0.0.1)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_TYPE "")

add_subdirectory(ExternalDependencies)
add_subdirectory(CoreV3)
add_subdirectory(CoreV3Test)
add_subdirectory(GameEngine)
add_subdirectory(GameEngine_Editor)
add_subdirectory(SandboxApplication)

add_subdirectory(Test)