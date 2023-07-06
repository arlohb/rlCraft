cmake_minimum_required(VERSION 3.25.3)

############################
#         COMPILER         #
############################

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

############################
#       DEPENDENCIES       #
############################

# Import this cmake module
include(FetchContent)

# TODO make sure this only affects dependencies
add_compile_options(-Wno-everything)

# Fetch raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

# Fetch raylib-cpp
FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp
    GIT_TAG v4.5.1
)
FetchContent_MakeAvailable(raylib_cpp)

############################
#         PROJECT          #
############################

# Set project compiler options
add_compile_options(-Wall -Wextra -Wpedantic)

# Setup the project
project(rlCraft)
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link required libraries
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)

# Copy assets to the build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Symlink compile_commands to root dir
file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
)

