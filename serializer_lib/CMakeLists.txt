set(target_name "serializer_lib")

set(src_serializer
    "src/serializer/Buffer.h"
    "src/serializer/Serializer.h")

set(src_files
    ${src_serializer}
    "src/dummy.cpp")

add_library(${target_name} STATIC ${src_files})

target_include_directories(${target_name} INTERFACE "src")

source_group("src\\serializer" FILES ${src_serializer})

set_target_properties(${target_name} PROPERTIES FOLDER "serializer")