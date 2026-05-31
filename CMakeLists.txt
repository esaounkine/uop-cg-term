cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME term-project)

set(SOURCES main.cpp visuals.cpp texture.cpp terrain.cpp BMPLoader.cpp)

project(${PROJECTNAME})

add_executable(${PROJECTNAME} ${SOURCES})

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECTNAME} OpenGL::GL GLUT::GLUT OpenGL::GLU)

file(COPY models DESTINATION .)

