cmake_minimum_required(VERSION 3.1.3)

project(chromakey VERSION 0.1 LANGUAGES CXX)

find_package(OpenCV REQUIRED core videoio imgproc imgcodecs highgui)

set(CMAKE_CXX_STANDARD 11)

add_library(chromakey chromakey.hpp chromakey.cpp)

target_include_directories(chromakey PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(chromakey PUBLIC ${OpenCV_LIBS})


add_executable(main main.cpp)
target_link_libraries(main PUBLIC chromakey ${OpenCV_LIBS})

add_executable(mainVideo mainVideo.cpp)
target_link_libraries(mainVideo PUBLIC chromakey ${OpenCV_LIBS})
