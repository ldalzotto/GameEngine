cmake_minimum_required(VERSION 3.12)
project(PNGSerializer VERSION 0.0.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(PNGSerializer "PNGSerializer.c")

target_link_libraries(PNGSerializer PUBLIC Core_c)
target_link_libraries(PNGSerializer PUBLIC Math)
target_link_libraries(PNGSerializer PUBLIC Render_Asset)
#target_link_libraries(PNGSerializer PUBLIC ZLib)
target_link_libraries(PNGSerializer PUBLIC STB_IMAGE)

#copy_zlib_dll(${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
