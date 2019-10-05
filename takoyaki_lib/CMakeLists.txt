set(target_name "takoyaki_lib")

set(graphics_gl_files
    "src/takoyaki/graphics/gl/RenderTarget.h"
    "src/takoyaki/graphics/gl/RenderTarget.cpp"
    "src/takoyaki/graphics/gl/Shader.h"
    "src/takoyaki/graphics/gl/ShaderProgram.h"
    "src/takoyaki/graphics/gl/RenderCommands.h"
    "src/takoyaki/graphics/gl/RenderCommand.h")

set(graphics_files
    "src/takoyaki/graphics/Renderer.h"
    "src/takoyaki/graphics/Renderer.cpp"
    "src/takoyaki/graphics/BaseWindow.h"
    "src/takoyaki/graphics/MainWindow.h"
    "src/takoyaki/graphics/MainWindow.cpp")

set(editor_files
    "src/takoyaki/editor/handlers/Handlers.h"
    "src/takoyaki/editor/MainEditor.h"
    "src/takoyaki/editor/MainEditor.cpp"
    "src/takoyaki/editor/ServiceManager.h"
    "src/takoyaki/editor/ServiceManager.cpp"
    "src/takoyaki/editor/Takoyaki.h"
    "src/takoyaki/editor/Takoyaki.cpp")

set(editor_components_files
    "src/takoyaki/editor/components/Widget.cpp"
    "src/takoyaki/editor/components/Widget.h"
    "src/takoyaki/editor/components/Animator/Animator.cpp"
    "src/takoyaki/editor/components/Animator/Animator.h"
    "src/takoyaki/editor/components/Timeline/Timeline.cpp"
    "src/takoyaki/editor/components/Timeline/Timeline.h"
    "src/takoyaki/editor/components/DockSpace.cpp"
    "src/takoyaki/editor/components/DockSpace.h"
    "src/takoyaki/editor/components/Preview.h"
    "src/takoyaki/editor/components/Preview.cpp"
    "src/takoyaki/editor/components/Camera.h"
    "src/takoyaki/editor/components/Camera.cpp"
    "src/takoyaki/editor/components/UniformsMenu.h"
    "src/takoyaki/editor/components/UniformsMenu.cpp"
    "src/takoyaki/editor/components/UniformItem.h")

set(editor_systems_files
    "src/takoyaki/editor/systems/project/FileLocator.h"
    "src/takoyaki/editor/systems/project/FileLocator.cpp"
    "src/takoyaki/editor/systems/project/ProjectSystem.h"
    "src/takoyaki/editor/systems/project/ProjectSystem.cpp"
    "src/takoyaki/editor/systems/scene/SceneSystem.h"
    "src/takoyaki/editor/systems/scene/SceneSystem.cpp"
    "src/takoyaki/editor/systems/sync/SyncSystem.h"
    "src/takoyaki/editor/systems/sync/SyncSystem.cpp"
    "src/takoyaki/editor/systems/animator/Easings.h"
    "src/takoyaki/editor/systems/animator/Easings.cpp"
    "src/takoyaki/editor/systems/animator/KeyFrameList.h"
    "src/takoyaki/editor/systems/animator/KeyFrameList.cpp"
    "src/takoyaki/editor/systems/animator/AnimationTrack.h"
    "src/takoyaki/editor/systems/animator/AnimationTrack.cpp"
    "src/takoyaki/editor/systems/animator/AnimationSystem.h"
    "src/takoyaki/editor/systems/animator/AnimationSystem.cpp"
    "src/takoyaki/editor/systems/music/MusicSystem.h"
    "src/takoyaki/editor/systems/music/MusicSystem.cpp")

set(util_files
    "src/takoyaki/util/FileWatcher.h"
    "src/takoyaki/util/FileWatcher.cpp"
    "src/takoyaki/util/Input.h"
    "src/takoyaki/util/glmvecToImVec.h"
    "src/takoyaki/util/VariantPolyfill.h")

set(precompiled_files
    "src/takoyaki/precompiled.h")

set(app_files
    "src/takoyaki/TakoyakiApp.h"
    "src/takoyaki/TakoyakiApp.cpp")

set(src_files
    ${graphics_gl_files}
    ${graphics_files}
    ${editor_files}
    ${editor_components_files}
    ${editor_systems_files}
    ${util_files}
    ${precompiled_files}
    ${app_files})

add_library(${target_name} STATIC ${src_files})

target_link_libraries(${target_name} PUBLIC
    serializer_lib
    glfw
    glad
    glm_static
    imgui
    tinyfd
    bass
    compiler-warnings)

target_include_directories(${target_name} INTERFACE "src")

source_group("src\\app" FILES ${app_files})
source_group("src\\precompiled" FILES ${precompiled_files})
source_group("src\\editor" FILES ${editor_files})
source_group("src\\editor\\components" FILES ${editor_components_files})
source_group("src\\editor\\systems" FILES ${editor_systems_files})
source_group("src\\graphics" FILES ${graphics_files})
source_group("src\\graphics\\gl" FILES ${graphics_gl_files})
source_group("src\\util" FILES ${util_files})


set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${precompiled_files})

cotire(${target_name})