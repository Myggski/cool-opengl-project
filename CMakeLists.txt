cmake_minimum_required(VERSION 3.25.0)
project(cool-opengl VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

file(GLOB_RECURSE SRC_COOL_OPENGL
    "src/*.cpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME}
    ${SRC_COOL_OPENGL}
)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive 
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}.")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed.")
endif()

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)

add_subdirectory(
    "${CMAKE_SOURCE_DIR}/external/glfw"
)

add_subdirectory( 
    "${CMAKE_SOURCE_DIR}/external/glew-cmake"
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${OPENGL_INCLUDE_DIRS}
    PUBLIC external/glfw/include
    PUBLIC external/glew-cmake/include
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw
    PRIVATE libglew_static
)

if (MSVC)
    source_group("src" FILES ${SRC_COOL_OPENGL})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()