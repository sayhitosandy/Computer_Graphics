cmake_minimum_required(VERSION 3.6)
project(Assignment02_code)



set(CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include(HelperFunctions)



set(CMAKE_CXX_STANDARD 11)

file(GLOB DEPS_SOURCES ext/glad/src/glad.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)

include_directories("ext/glm/"
        "ext/glfw/include"
        "ext/glad/include"
        "src"
        )
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(ext/glfw)

set(PLATFORM_LINK )
if (UNIX)
    set(PLATFORM_LINK -ldl)
endif (UNIX)
file(GLOB SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB STUDENT_CODE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/StudentCode/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/StudentCode/*.hpp")

add_executable(StudentCode ${STUDENT_CODE} ${SOURCE_FILES} ${DEPS_SOURCES})
target_link_libraries(StudentCode glfw ${GLFW_LIBRARIES} ${PLATFORM_LINK})
pre_build(StudentCode
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Resources)
