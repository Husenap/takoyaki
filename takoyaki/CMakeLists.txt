set(target_name "takoyaki")

set(src_files
    "src/main.cpp")

add_executable(${target_name} ${src_files})

target_link_libraries(${target_name} PUBLIC glfw)

FetchContent_GetProperties(imgui SOURCE_DIR imgui_src_location)
target_include_directories(${target_name} PUBLIC ${imgui_src_location})


set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_HOME_DIRECTORY}/bin )
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )