set(target_name "bass_lib")

add_library(${target_name} STATIC "dummy.cpp" "include/bass.h")
target_include_directories(${target_name} PUBLIC "include/")
target_link_libraries(${target_name} "${CMAKE_CURRENT_SOURCE_DIR}/bass.lib")