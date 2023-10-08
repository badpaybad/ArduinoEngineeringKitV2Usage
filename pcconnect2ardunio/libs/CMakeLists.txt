cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME "serialport")
project(${PROJECT_NAME} LANGUAGES C)


add_library(${PROJECT_NAME} SHARED
  "tcgets.c"
  "tcgets.h"
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -std=c99 -Wall -Wextra -pedantic -Wmissing-prototypes -Wshadow)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBSERIALPORT_ATBUILD)
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}")
