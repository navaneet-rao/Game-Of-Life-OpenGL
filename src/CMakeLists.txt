# Set sources in the src directory
file(GLOB SOURCES "*.cpp")

# Add executable from sources
add_executable(Conway ${SOURCES})

# Include directories for the executable
target_include_directories(Conway PRIVATE
    ${CMAKE_SOURCE_DIR}/menu/ImGui  # Include ImGui headers from ImGui folder
    ${CMAKE_SOURCE_DIR}/menu/ImGui/backends  # Include ImGui headers from ImGui folder
)

# Link libraries
target_link_libraries(Conway
    OpenGL::OpenGL
    GLUT::GLUT
    GLU
)

