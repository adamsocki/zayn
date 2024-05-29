cmake_minimum_required(VERSION 3.10)

# Set project name
project(ZaynEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ZaynEngine src/main.cpp)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Find GLFW package
find_package(glfw3 3.3 REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

# Link libraries
target_link_libraries(ZaynEngine ${OPENGL_LIBRARIES} glfw)
