set(target_name "takoyaki")

set(graphics_gl_files
    "src/graphics/gl/RenderTarget.h"
    "src/graphics/gl/RenderTarget.cpp"
    "src/graphics/gl/Shader.h"
    "src/graphics/gl/ShaderProgram.h"
    "src/graphics/gl/RenderCommands.h"
    "src/graphics/gl/RenderCommand.h")

set(graphics_files
    "src/graphics/Renderer.h"
    "src/graphics/Renderer.cpp"
    "src/graphics/BaseWindow.h"
    "src/graphics/MainWindow.h"
    "src/graphics/MainWindow.cpp")

set(editor_files
    "src/editor/handlers/Handlers.h"
    "src/editor/MainEditor.h"
    "src/editor/MainEditor.cpp"
    "src/editor/Takoyaki.h"
    "src/editor/Takoyaki.cpp")

set(editor_components_files
    "src/editor/components/Preview.h"
    "src/editor/components/Preview.cpp"
    "src/editor/components/Camera.h"
    "src/editor/components/Camera.cpp"
    "src/editor/components/UniformsMenu.h"
    "src/editor/components/UniformsMenu.cpp"
    "src/editor/components/UniformItem.h")

set(util_files
    "src/util/FileWatcher.h"
    "src/util/FileWatcher.cpp"
    "src/util/Input.h"
    "src/util/glmvecToImVec.h"
    "src/util/VariantPolyfill.h")

set(precompiled_files
    "src/precompiled.h")

set(src_files
    ${graphics_gl_files}
    ${graphics_files}
    ${editor_files}
    ${editor_components_files}
    ${util_files}
    ${precompiled_files}
    "src/main.cpp")

add_executable(${target_name} ${src_files})

target_link_libraries(${target_name} PUBLIC glfw glad glm_static imgui tinyfd compiler-warnings)

source_group("src" FILES "src/main.cpp")
source_group("src\\precompiled" FILES ${precompiled_files})
source_group("src\\editor" FILES ${editor_files})
source_group("src\\editor\\components" FILES ${editor_components_files})
source_group("src\\graphics" FILES ${graphics_files})
source_group("src\\graphics\\gl" FILES ${graphics_gl_files})
source_group("src\\util" FILES ${util_files})


set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")
#set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
#set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_HOME_DIRECTORY}/bin )
#set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_HOME_DIRECTORY}/bin )
#set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_HOME_DIRECTORY}/bin )
#set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${precompiled_files})

cotire(${target_name})