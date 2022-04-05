cmake_minimum_required(VERSION 3.21)
project(graphics)

set(CMAKE_CXX_STANDARD 14)

add_executable(graphics src/main.cpp src/dispatch.cpp src/dispatch.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})