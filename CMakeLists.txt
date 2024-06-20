cmake_minimum_required(VERSION 3.10)
project(Conway)

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")

# Include directories
include_directories(
    ${Conway_SOURCE_DIR}/include      # Assuming include directory is in the root
    ${GLUT_INCLUDE_DIRS}              # Include GLUT directories if necessary
    ${PROJECT_SOURCE_DIR}/menu/ImGui  # Include ImGui headers from the menu folder
    ${PROJECT_SOURCE_DIR}/menu/ImGui/backends  # Include ImGui headers from the menu folder
)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)

# Add subdirectory for source files
add_subdirectory(src)

