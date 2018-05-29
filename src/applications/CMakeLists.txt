add_executable(main main.cpp)
target_link_libraries(main PUBLIC chromakey ${OpenCV_LIBS})

add_executable(mainVideo mainVideo.cpp)
target_link_libraries(mainVideo PUBLIC chromakey ${OpenCV_LIBS})

install(TARGETS main mainVideo
        EXPORT "${targets_export_name}"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        INCLUDES DESTINATION "${include_install_dir}"
)