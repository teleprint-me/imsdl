cmake_minimum_required(VERSION 3.17)
project("imsdl" LANGUAGES C)

set(CMAKE_C_STANDARD 17)

# Add sanitizers for memory safety
# Ref: https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
# Ref: https://developers.redhat.com/blog/2018/03/21/compiler-and-linker-flags-gcc

# Common warning flags
set(COMMON_WARNING_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wformat-security -Wshadow -fexceptions")

# Additional Debug-only flags (sanitizers and memory safety checks)
set(DEBUG_SANITIZERS "-fsanitize=address,undefined -fno-omit-frame-pointer")
set(DEBUG_EXTRA_WARNINGS "-Wformat -Wnull-dereference -Wdouble-promotion")

# Static analysis flags for catching common security issues
set(DEBUG_ANALYSIS "-Wanalyzer-double-free -Wanalyzer-file-leak -Wanalyzer-malloc-leak -Wanalyzer-null-dereference -Wanalyzer-out-of-bounds -Wanalyzer-va-list-leak")

# Enable debugging level 3 for macros
set(DEBUG_FLAGS "-g3")

# Apply flags conditionally based on build type
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_WARNING_FLAGS} ${DEBUG_SANITIZERS} ${DEBUG_EXTRA_WARNINGS} ${DEBUG_ANALYSIS} ${DEBUG_FLAGS}")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_WARNING_FLAGS}")
endif()

# Enable Shared Libraries option
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Add executable
add_executable(imsdl src/main.c)

# Link SDL2, OpenGL, GLFW, and GLEW
target_link_libraries(imsdl m asan SDL2 GL glfw GLEW::GLEW)
