# Example of minimal CMakeLists.txt for menu directory

# Add ImGui source files
file(GLOB IMGUI_SOURCES "*.cpp")

# Add ImGui header directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library from ImGui sources
add_library(ImGui ${IMGUI_SOURCES})

# Optionally, link ImGui library to any other target that needs it
# Example:
# target_link_libraries(SomeTarget ImGui)

