cmake_minimum_required(VERSION 3.17)
project("imsdl" LANGUAGES C)

set(CMAKE_C_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Add executable
add_executable(imsdl src/main.c)

# Link SDL2, OpenGL, GLFW, and GLEW
target_link_libraries(imsdl m SDL2 GL glfw GLEW::GLEW)
