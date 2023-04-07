cmake_minimum_required(VERSION 3.10)

project(chromakey VERSION 0.1 LANGUAGES CXX)

option(BUILD_APPS "Build sample applications" ON)
option(BUILD_SHARED_LIBS "Build shared libs" ON)

find_package(OpenCV 3.0 REQUIRED core videoio imgproc imgcodecs highgui)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

add_subdirectory(src)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()