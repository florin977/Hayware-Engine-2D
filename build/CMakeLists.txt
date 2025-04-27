cmake_minimum_required(VERSION 3.11)
project(HaywareEngine C)

set(CMAKE_C_COMPILER gcc)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Use pkg-config for cglm
find_package(PkgConfig REQUIRED)
pkg_check_modules(CGLM REQUIRED cglm)
include_directories(${CGLM_INCLUDE_DIRS})

# Set paths
set(PARENT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
include_directories(${PARENT_DIRECTORY}/include)

set(SOURCES
    ${PARENT_DIRECTORY}/src/main.c
    ${PARENT_DIRECTORY}/src/utils.c
    ${PARENT_DIRECTORY}/src/breakout.c
    ${PARENT_DIRECTORY}/src/stb_image.c
    ${PARENT_DIRECTORY}/src/SDL/init_window.c
    ${PARENT_DIRECTORY}/src/render.c
    ${PARENT_DIRECTORY}/src/demo.c
)

add_executable(prg ${SOURCES})

# Link everything properly
target_link_libraries(prg OpenGL::OpenGL SDL3::SDL3 GLEW::GLEW m ${CGLM_LIBRARIES})