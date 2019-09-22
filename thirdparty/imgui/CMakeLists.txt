set(imgui_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(imgui_sources
    "imgui/imconfig.h"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/examples/imgui_impl_glfw.h"
    "imgui/examples/imgui_impl_glfw.cpp"
    "imgui/examples/imgui_impl_opengl3.h"
    "imgui/examples/imgui_impl_opengl3.cpp"
    "imgui/imgui_internal.h"
    "imgui/imgui_widgets.cpp"
    "imgui/imgui.cpp"
    "imgui/imgui.h"
    "imgui/imstb_rectpack.h"
    "imgui/imstb_textedit.h"
    "imgui/imstb_truetype.h"
)

add_library(imgui STATIC ${imgui_sources})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(imgui PUBLIC ${imgui_include_dir})

target_link_libraries(imgui glfw glad)
set_target_properties(imgui PROPERTIES FOLDER "thirdparty")