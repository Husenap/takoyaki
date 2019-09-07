set(target_name "takoyaki")

set(graphics_files
    "src/graphics/Commands.h"
    "src/graphics/Shader.h"
    "src/graphics/ShaderProgram.h")

set(precompiled_files
    "src/precompiled.h")

set(src_files
    ${graphics_files}
    ${precompiled_files}
    "src/main.cpp")

add_executable(${target_name} ${src_files})

target_link_libraries(${target_name} PUBLIC glfw glad imgui)

source_group("src" FILES "src/main.cpp")
source_group("src\\precompiled" FILES ${precompiled_files})
source_group("src\\graphics" FILES ${graphics_files})


set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${precompiled_files})

cotire(${target_name})