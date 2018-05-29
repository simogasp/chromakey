add_executable(main main.cpp)
target_link_libraries(main PUBLIC chromakey ${OpenCV_LIBS})

add_executable(mainVideo mainVideo.cpp)
target_link_libraries(mainVideo PUBLIC chromakey ${OpenCV_LIBS})