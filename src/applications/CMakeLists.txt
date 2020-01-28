add_executable(processImage processImage.cpp)
target_link_libraries(processImage PUBLIC chromakey ${OpenCV_LIBS})

add_executable(processVideo processVideo.cpp)
target_link_libraries(processVideo PUBLIC chromakey ${OpenCV_LIBS})

install(TARGETS processImage processVideo
        EXPORT "${targets_export_name}"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION "${include_install_dir}"
)