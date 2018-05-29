cmake_minimum_required(VERSION 3.1.3)

project(chromakey VERSION 0.1 LANGUAGES CXX)

find_package(OpenCV 3.0 REQUIRED core videoio imgproc imgcodecs highgui)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)