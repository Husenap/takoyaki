set(target_name "takoyaki_tests")

set(src_files
    "src/EasingsTests.cpp"
    "src/KeyFrameListTests.cpp")

add_executable(${target_name} ${src_files})

source_group("src" FILES ${src_files})

target_link_libraries(${target_name}
    "takoyaki_lib"
    "gtest_main")

set_target_properties(${target_name} PROPERTIES FOLDER "takoyaki")

gtest_discover_tests(${target_name})