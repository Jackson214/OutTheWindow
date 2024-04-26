cmake_minimum_required(VERSION 3.0)
project(OutTheWindow)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find OpenGL
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

# Add GLUT include directory
include_directories(
    ${CMAKE_SOURCE_DIR}/GLUT/include
)

# Add executable
add_executable(OutTheWindow 
    main.cpp
)

target_link_directories(OutTheWindow PUBLIC ${CMAKE_SOURCE_DIR}/GLUT/lib/x64)

# Link OpenGL and GLUT libraries
target_link_libraries(OutTheWindow 
    OpenGL::GL
    ${GLU_LIBRARY}
    freeglut
)



