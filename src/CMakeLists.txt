# Set sources in the src directory
file(GLOB SOURCES "*.cpp")

# Add executable from sources
add_executable(Conway ${SOURCES})

# Link libraries
target_link_libraries(Conway
    OpenGL::OpenGL
    GLUT::GLUT
    GLU
)

