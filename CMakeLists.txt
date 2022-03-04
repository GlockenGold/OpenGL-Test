cmake_minimum_required(VERSION 3.0)
project(OpenGL-Test
        VERSION 1.0
        DESCRIPTION "A C++ project for learning OpenGL"
        LANGUAGES CXX)

set(SOURCES_MAIN src/main.cpp include/shader.h src/shader.cpp include/stb_image.h src/stb_image.cpp include/camera.h src/camera.cpp)
add_executable(main ${SOURCES_MAIN})
target_include_directories(main PRIVATE include res/shaders res/textures)
target_compile_features(main PRIVATE cxx_std_20)

set(SOURCES_TRIANGLES src/two_triangles.cpp include/shader.h src/shader.cpp include/stb_image.h src/stb_image.cpp include/camera.h src/camera.cpp)
add_executable(triangles ${SOURCES_TRIANGLES})
target_include_directories(triangles PRIVATE include res/shaders res/textures)
target_compile_features(triangles PRIVATE cxx_std_20)

set(SOURCES_LIGHTING src/lighting_playground.cpp include/shader.h src/shader.cpp include/stb_image.h src/stb_image.cpp include/camera.h src/camera.cpp)
add_executable(lighting_playground ${SOURCES_LIGHTING})
target_include_directories(lighting_playground PRIVATE include res/shaders res/textures)
target_compile_features(lighting_playground PRIVATE cxx_std_20)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(main PRIVATE /W4)
    target_compile_options(triangles PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(triangles PRIVATE -Wall -Wextra -Wpedantic)
endif()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(main glfw)
target_link_libraries(triangles glfw)
target_link_libraries(lighting_playground glfw)

add_subdirectory(lib/glad)
target_link_libraries(main glad)
target_link_libraries(triangles glad)
target_link_libraries(lighting_playground glad)

add_subdirectory(lib/glm)
target_link_libraries(main glm)
target_link_libraries(triangles glm)
target_link_libraries(lighting_playground glm)
