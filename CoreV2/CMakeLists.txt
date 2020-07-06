cmake_minimum_required(VERSION 3.12)
project(CoreV2 VERSION 0.0.1)


add_library(CoreV2 "Includes/Build.c" "Includes/CoreV2Include.h")
target_include_directories(CoreV2 PUBLIC "Includes")
