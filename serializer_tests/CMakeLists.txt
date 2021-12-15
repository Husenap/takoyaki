set(target_name "serializer_tests")

set(src_files
    "src/SerializerTests.cpp")

add_executable(${target_name} ${src_files})

source_group("src" FILES ${src_files})

target_link_libraries(${target_name}
    "serializer_lib"
    "gmock_main")

set_target_properties(${target_name} PROPERTIES FOLDER "serializer")

gtest_discover_tests(${target_name})