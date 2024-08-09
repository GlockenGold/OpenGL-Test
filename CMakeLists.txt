cmake_minimum_required(VERSION 3.0)
project(OpenGL-Test
        VERSION 1.0
        DESCRIPTION "A C++ project for learning OpenGL"
        LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)
# find the required packages
# Technically a more appropriate approach, but I would need to modify the Find*.cmake files for it to work.
# find_package(GLM REQUIRED)
# message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
# find_package(GLFW3 REQUIRED)
# message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
# find_package(ASSIMP REQUIRED)
# message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)
add_subdirectory(lib/assimp)
set(LIBS glfw glad glm assimp)

# I don't know whether this approach is better than the one I'm using further down or not.
# if(WIN32)
#   set(LIBS glfw3 opengl32 assimp freetype irrKlang)
#   add_definitions(-D_CRT_SECURE_NO_WARNINGS)
# elseif(UNIX AND NOT APPLE)
#   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic")
#   find_package(OpenGL REQUIRED)
#   add_definitions(${OPENGL_DEFINITIONS})
#   find_package(X11 REQUIRED)
#   # note that the order is important for setting the libs
#   # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
#   set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
#   set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
# else()
#   set(LIBS )
# endif(WIN32)

# first create relevant static libraries requried for other projects
# I don't know how I should make this work. It doesn't find the header file in the include/ directory.
# add_library(STB_IMAGE "src/stb_image.cpp")
# set(LIBS ${LIBS} STB_IMAGE)

set(SOURCES_MAIN src/main.cpp include/stb_image.h src/stb_image.cpp include/shader.h src/shader.cpp include/camera.h src/camera.cpp)
add_executable(main ${SOURCES_MAIN})
target_include_directories(main PRIVATE include res/shaders res/textures res/objects)
target_compile_features(main PRIVATE cxx_std_20)

# set(SOURCES) should probably be replaced by a glob function, to make it easier to add more files.
set(SOURCES_LIGHTCASTERS src/light_casters.cpp include/stb_image.h src/stb_image.cpp include/shader.h src/shader.cpp include/camera.h src/camera.cpp include/mesh.h include/model.h)
add_executable(light_casters ${SOURCES_LIGHTCASTERS})
target_include_directories(light_casters PRIVATE include res/shaders res/textures res/objects)
target_compile_features(light_casters PRIVATE cxx_std_20)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(main PRIVATE /W4)
    target_compile_options(light_casters PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(light_casters PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(main ${LIBS})
target_link_libraries(light_casters ${LIBS})
