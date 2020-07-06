cmake_minimum_required(VERSION 3.12)
project(CoreV2Test VERSION 0.0.1)

add_executable(CoreV2Test 
	"Includes/Build.c" ##
 )

target_link_libraries(CoreV2Test CoreV2)