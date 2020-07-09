cmake_minimum_required(VERSION 3.12)
project(CoreV3 VERSION 0.0.1)


add_library(CoreV3 "Includes/Build.cpp" "Includes/CoreV3Include.hpp" #
   )
target_include_directories(CoreV3 PUBLIC "Includes")
