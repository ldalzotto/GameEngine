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

add_executable(RasterTest "src/RasterTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(RasterTest PUBLIC GameEngine)
target_link_libraries(RasterTest PUBLIC MyRender)
target_link_libraries(RasterTest PUBLIC Math)

add_executable(GizmoTest "src/GizmoTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(GizmoTest PUBLIC GameEngine)
target_link_libraries(GizmoTest PUBLIC MyRender)

add_executable(MathTest "src/MathTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(MathTest PUBLIC GameEngine)
target_link_libraries(MathTest PUBLIC Math)

add_executable(CustomAllocatorTest "src/CustomAllocatorTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(CustomAllocatorTest PUBLIC GameEngine)

add_executable(TextureLoadTest "src/TextureLoadTest.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(TextureLoadTest PUBLIC GameEngine)
target_link_libraries(TextureLoadTest PUBLIC MyRender)
#copy_zlib_dll(${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

add_executable(SoftwareRendererShowcase_icosphere "src/SoftwareRendererShowcase_icosphere.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(SoftwareRendererShowcase_icosphere PUBLIC GameEngine)
target_link_libraries(SoftwareRendererShowcase_icosphere PUBLIC MyRender)

add_executable(SoftwareRendererShowcase_tower "src/SoftwareRendererShowcase_tower.c" ${TEST_APP_PUBLIC} ${TEST_APP_PRIVATE})
target_link_libraries(SoftwareRendererShowcase_tower PUBLIC GameEngine)
target_link_libraries(SoftwareRendererShowcase_tower PUBLIC MyRender)