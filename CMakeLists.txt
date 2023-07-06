cmake_minimum_required(VERSION 3.25.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

project(rlCraft)

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(raylib-cpp)

target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
)

