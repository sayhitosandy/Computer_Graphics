cmake_minimum_required(VERSION 3.7)
set(AssignmentNameHere Assignment1)
project(${AssignmentNameHere})

set(CMAKE_CXX_STANDARD 11)
macro(pre_build TARGET_NAME)
    add_custom_target( ${TARGET_NAME}_PRE_BUILD ALL )
    message("STATUS" "current source dir in macro " ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(
            TARGET ${TARGET_NAME}_PRE_BUILD
            ${ARGN}
            PRE_BUILD
            COMMENT "${TARGET_NAME}_PRE_BUILD ..."
    )

    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_PRE_BUILD)
endmacro()

file(GLOB DEPS_SOURCES ext/glad/src/glad.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)
include_directories("ext/glm/"
        "ext/glfw/include"
        "ext/glad/include"
        "ext/imgui"


        )



option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(ext/glfw)


file(GLOB IMGUI_SOURCE
        ext/imgui/*.cpp
        ext/imgui/*.h
        )


set(PLATFORM_LINK )
if (UNIX)
    set(PLATFORM_LINK -ldl)
endif (UNIX)


set(SOURCE_FILES main.cpp src/Game.cpp src/Game.hpp src/utitliy.cpp src/utitliy.hpp )
add_executable(${AssignmentNameHere} ${SOURCE_FILES}  ${DEPS_SOURCES} ${IMGUI_SOURCE})
target_link_libraries(${AssignmentNameHere} glfw ${GLFW_LIBRARIES} ${PLATFORM_LINK} )

pre_build(${AssignmentNameHere}
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/bin/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory   ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_SOURCE_DIR}/bin/resources)