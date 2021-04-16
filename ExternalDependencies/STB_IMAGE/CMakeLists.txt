cmake_minimum_required(VERSION 3.12)
project(STB_IMAGE VERSION 1.0.0)

SET(STB_IMAGE_PRIVATE ${STB_IMAGE_PRIVATE} private/stb_image.c)
SET(STB_IMAGE_PUBLIC ${STB_IMAGE_PUBLIC} public/stb_image.h)

add_library(STB_IMAGE STATIC ${STB_IMAGE_PUBLIC} ${STB_IMAGE_PRIVATE})

target_include_directories(STB_IMAGE PUBLIC "public/")