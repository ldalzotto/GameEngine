cmake_minimum_required(VERSION 3.12)
project(CoreV3Test VERSION 0.0.1)

add_executable(CoreV3Test 
	"Includes/Build.cpp" ##
 )

target_link_libraries(CoreV3Test CoreV3)