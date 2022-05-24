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

add_executable(graphics1 lab1/main.cpp lab1/dispatch.cpp lab1/dispatch.h)

include_directories(${FREE_GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} lib)
#

target_link_libraries(graphics1 ${FREE_GLUT_LIBRARY} ${OPENGL_LIBRARIES})
#

add_executable(graphics2 lab2/main.cpp lab2/dispatch.cpp lab2/dispatch.h lib/Command.h lib/Shader.h lab2/Grid.h lab2/State.h)
target_link_libraries(graphics2 ${GLUT_LIBRARY} ${OPENGL_LIBRARIES} /usr/lib/libSOIL.so /usr/local/lib/libglut.so)


target_link_libraries(graphics2 ${GLFW_STATIC_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL})
link_libraries(${GLEW_LIBRARIES})
target_link_libraries(graphics2 glm::glm)

#link_directories("/usr/local")
