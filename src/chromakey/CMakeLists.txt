add_library(chromakey chromakey.hpp chromakey.cpp)

set(LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(chromakey
        PUBLIC
        $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/>
        ${OpenCV_INCLUDE_DIRS})
target_link_libraries(chromakey PUBLIC ${OpenCV_LIBS})

set_target_properties(chromakey PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(chromakey PROPERTIES VERSION ${PROJECT_VERSION})

if(MSVC)
    target_compile_definitions(chromakey PUBLIC -DNOMINMAX)
endif()

# Targets:
#   * header location after install: <prefix>/include/<PROJECT-NAME>/*.hpp
#   * headers can be included by C++ code `#include <<PROJECT-NAME>/*.hpp>`
install(
        TARGETS chromakey
        EXPORT "${targets_export_name}"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Headers:
install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/src/chromakey"
        DESTINATION "${include_install_dir}"
        FILES_MATCHING PATTERN "*.hpp"
)