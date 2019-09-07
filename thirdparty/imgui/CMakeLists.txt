set(imgui_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB imgui_sources ${imgui_include_dir}/*.cpp)
file(GLOB imgui_headers ${imgui_include_dir}/*.h)

add_library(imgui STATIC ${imgui_sources} ${imgui_headers})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(imgui PUBLIC ${imgui_include_dir})

target_link_libraries(imgui glfw glad)
set_target_properties(imgui PROPERTIES FOLDER "thirdparty")