cmake_minimum_required(VERSION 3.12)
project(SandboxApplication VERSION 0.0.1)

file(GLOB_RECURSE SANDBOX_APP_PRIVATE "./*.cpp")
file(GLOB_RECURSE SANDBOX_APP_PUBLIC "./*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SandboxApplication ${SANDBOX_APP_PUBLIC} ${SANDBOX_APP_PRIVATE})

target_include_directories(SandboxApplication PUBLIC "./")
target_link_libraries(SandboxApplication PUBLIC GameEngine)

copy_assimp_dll(${PROJECT_BINARY_DIR}/${BUILD_TYPE}Debug)