set(target_name "takoyaki")

set(src_files
    "src/main.cpp")

add_executable(${target_name} ${src_files})

target_link_libraries(${target_name} "takoyaki_lib")

source_group("src" FILES "src/main.cpp")

set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )