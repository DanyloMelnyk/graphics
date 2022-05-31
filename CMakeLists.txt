cmake_minimum_required(VERSION 3.21)
project(graphics1)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(FreeGLUT REQUIRED)
find_package(GLEW REQUIRED)
set(CMAKE_CXX_FLAGS "-lSOIL")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(glm REQUIRED)

include_directories(${FREE_GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} lib)

link_libraries(${GLEW_LIBRARIES} /usr/local/lib/libglut.so ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${GLEW_LIBRARIES} /usr/lib/libSOIL.so glm::glm)

add_executable(graphics1 lab1/main.cpp lab1/dispatch.cpp lab1/dispatch.h)

add_executable(graphics2 lab2/main.cpp lab2/dispatch.cpp lab2/dispatch.h lib/Command.h lib/Shader.h lab2/Grid.h lab2/State.h)

add_executable(graphics3 lab3/src/main.cpp lab3/src/dispatch.h lab3/src/dispatch.cpp lib/Command.h lib/Shader.h lab2/Grid.h lab2/State.h lab3/src/Camera.h lab3/src/Figure.cpp lab3/src/Figure.h lab3/src/CrossSection.cpp lab3/src/CrossSection.h lab3/src/Material.cpp lab3/src/Material.h lab3/src/Light.cpp lab3/src/Light.h lab3/src/Trace.cpp lab3/src/Trace.h)

