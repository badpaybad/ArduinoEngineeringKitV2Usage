cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
#fix: error C2039: 'string_view': should c++17
set(CMAKE_CXX_STANDARD_REQUIRED on)

set( CMAKE_CXX_FLAGS " -pthread " )

project (pcconnect2ardunio)
#version
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 1)

message("")
message("PROJECT_NAME: ${PROJECT_NAME}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("")

file (GLOB PRJ_LIBS_SOURCE 
"libs/*.h"
"libs/*.cpp"
)
add_library (PRJ_LIBS ${PRJ_LIBS_SOURCE})
add_subdirectory (libs)
include_directories(libs)

add_executable(${PROJECT_NAME} "pcconnect2ardunio.cpp")

target_link_libraries(${PROJECT_NAME} PRJ_LIBS)

file(GLOB PRJ_LIBS_HEADERS "libs/*.h")
file(COPY ${PRJ_LIBS_HEADERS} DESTINATION "libs")

#can create folder build
#cd build
#cmake ..
#cmake --build .

#cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="/vcpkg/scripts/buildsystems/vcpkg.cmake"

