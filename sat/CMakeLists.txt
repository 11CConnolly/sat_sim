# https://libcheck.github.io/check/
cmake_minimum_required(VERSION 3.5)

project(c_sat)

# Custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure whether libraries will be static or shared linked
set(BUILD_SHARED_LIBS OFF)

# Determine minimum C
set(CMAKE_C_STANDARD 11)

# Enable debugging
set(CMAKE_BUILD_TYPE Debug)

# Show all warning messages
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
