add_library(chromakey chromakey.hpp chromakey.cpp)

target_include_directories(chromakey PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(chromakey PUBLIC ${OpenCV_LIBS})

set_target_properties(chromakey PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(chromakey PROPERTIES VERSION ${PROJECT_VERSION})

# Targets:
#   * header location after install: <prefix>/include/<PROJECT-NAME>/*.hpp
#   * headers can be included by C++ code `#include <<PROJECT-NAME>/*.hpp>`
install(
        TARGETS chromakey
        EXPORT "${targets_export_name}"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/src/chromakey"
        DESTINATION "${include_install_dir}"
        FILES_MATCHING PATTERN "*.hpp"
)