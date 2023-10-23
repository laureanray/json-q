cmake_minimum_required(VERSION 3.10)

project(jsonq)

# Set the C standard to C11
set(CMAKE_C_STANDARD 11)

# Add the source files in the src/ directory
file(GLOB SOURCES "src/*.c")

# Include the src/ directory for header files
include_directories("src")

# Create the executable
add_executable(jsonq ${SOURCES})
