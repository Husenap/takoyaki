set(target_name "serializer_tests")

set(src_files
    "src/main.cpp")

add_executable(${target_name} ${src_files})

source_group("src" FILES ${src_files})

target_link_libraries(${target_name} "serializer_lib")
target_link_libraries(${target_name} "gtest_main")

set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")

gtest_discover_tests(${target_name})