cmake_minimum_required(VERSION 3.25.3)

# Import this cmake module
include(FetchContent)

# Set compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

# Setup the project
project(rlCraft)
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Fetch raylib-cpp
FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp
    GIT_TAG v4.5.1
)
FetchContent_MakeAvailable(raylib_cpp)

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

