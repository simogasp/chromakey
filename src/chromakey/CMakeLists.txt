add_library(chromakey chromakey.hpp chromakey.cpp)

target_include_directories(chromakey PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(chromakey PUBLIC ${OpenCV_LIBS})

set_target_properties(chromakey PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(chromakey PROPERTIES VERSION ${PROJECT_VERSION})
