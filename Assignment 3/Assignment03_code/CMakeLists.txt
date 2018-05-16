cmake_minimum_required(VERSION 3.7)
set(AssignmentNameHere Assignment3)
project(${AssignmentNameHere})


set(CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include(HelperFunctions)

set(CMAKE_CXX_STANDARD 11)

file(GLOB DEPS_SOURCES ext/glad/src/glad.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)



include_directories("ext/glm/"
        "ext/glfw/include"
        "ext/glad/include"
        )
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(ext/glfw)

set(PLATFORM_LINK )
if (UNIX)
    set(PLATFORM_LINK -ldl)
endif (UNIX)
#TODO add your source files here
set(SOURCE_FILES src/main.cpp src/gl_utils.cpp src/gl_utils.h  src/shader_utils.cpp src/shader_utils.h)

add_executable(${AssignmentNameHere} ${SOURCE_FILES} ${DEPS_SOURCES})
target_link_libraries(${AssignmentNameHere} glfw ${GLFW_LIBRARIES} ${PLATFORM_LINK})
pre_build(${AssignmentNameHere}
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)
