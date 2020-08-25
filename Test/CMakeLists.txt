cmake_minimum_required(VERSION 3.12)
project(Test VERSION 0.0.1)

file(GLOB_RECURSE TEST_APP_PRIVATE "./Lib/*.c" "./Lib/*.cpp")
file(GLOB_RECURSE TEST_APP_PUBLIC "./Lib/*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(PhysicsRaycastTest "src/PhysicsRaycastTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(PhysicsRaycastTest PUBLIC GameEngine)

add_executable(EntitySelectionTest "src/EntitySelectionTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(EntitySelectionTest PUBLIC GameEngine)

add_executable(CameraMatricesTest  ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(CameraMatricesTest PUBLIC GameEngine)

add_executable(MyRenderTest "src/MyRenderTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(MyRenderTest PUBLIC GameEngine)
target_link_libraries(MyRenderTest PUBLIC MyRender)
