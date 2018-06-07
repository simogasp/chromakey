cmake_minimum_required(VERSION 3.1.3)

project(chromakey VERSION 0.1 LANGUAGES CXX)

option(BUILD_APPS "Build sample applications" ON)

find_package(OpenCV 3.0 REQUIRED core videoio imgproc imgcodecs highgui)

set(CMAKE_CXX_STANDARD 11)

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