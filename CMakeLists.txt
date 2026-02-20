# This CMakeLists.txt works on Windows (Visual Studio) and Linux (clang/gcc).
# It fetches and builds GLFW automatically if not found on the system.
# The working directory is set to the project root, so resources in res/ are found at runtime.

cmake_minimum_required(VERSION 3.15)
set(CMAKE_VS_STARTUP_PROJECT Pseudocraft)
project(Pseudocraft)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output to top‑level bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_DEBUG_POSTFIX "_d")

add_executable(${PROJECT_NAME})

# Set working directory to project root for debugging
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

find_package(OpenGL REQUIRED)

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found locally, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
    set(GLFW_TARGET glfw)
else()
    set(GLFW_TARGET glfw)
endif()

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    ${GLFW_TARGET}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32)
endif()

# Optional: copy only if you need a standalone release build
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_SOURCE_DIR}/res
#         ${CMAKE_SOURCE_DIR}/bin/res
#     COMMENT "Copying resources to bin/"
# )