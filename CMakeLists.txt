cmake_minimum_required(VERSION 3.0)
project(test
        VERSION 1.0
        DESCRIPTION "A C++ project for learning OpenGL"
        LANGUAGES CXX)

set(SOURCES src/main.cpp include/shader.h)
add_executable(test ${SOURCES})
target_include_directories(test PRIVATE include)
target_compile_features(test PRIVATE cxx_std_20)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(test PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(test glfw)

add_subdirectory(lib/glad)
target_link_libraries(test glad)
