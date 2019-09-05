set(nuklear_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(nuklear STATIC
    "include/nuklear.cpp"
    "include/nuklear.h"
    "include/nuklear_glfw_gl4.h")

target_include_directories(nuklear PUBLIC ${nuklear_include_dir})

target_link_libraries(nuklear glfw glad)