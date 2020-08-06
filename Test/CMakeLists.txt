cmake_minimum_required(VERSION 3.12)
project(Test VERSION 0.0.1)

file(GLOB_RECURSE TEST_APP_PRIVATE "./Lib/*.cpp")
file(GLOB_RECURSE TEST_APP_PUBLIC "./Lib/*.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Test "./src/TransformTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(Test PUBLIC GameEngine)

add_executable(PhysicsRaycastTest "src/PhysicsRaycastTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(PhysicsRaycastTest PUBLIC GameEngine)

add_executable(EntitySelectionTest "src/EntitySelectionTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(EntitySelectionTest PUBLIC GameEngine)

add_executable(MathTest "src/MathTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(MathTest PUBLIC GameEngine)
target_link_libraries(MathTest PUBLIC Math)

add_executable(CameraMatricesTest "src/CameraMatricesTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(CameraMatricesTest PUBLIC GameEngine)

add_executable(MyRenderTest "src/MyRenderTest.cpp" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(MyRenderTest PUBLIC GameEngine)
target_link_libraries(MyRenderTest PUBLIC MyRender)

copy_assimp_dll(${PROJECT_BINARY_DIR}/${BUILD_TYPE}/Debug)