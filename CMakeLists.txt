cmake_minimum_required(VERSION 3.21)

project("takoyaki" VERSION "0.1.0" LANGUAGES CXX)

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")
include("cmake/auto_group_sources.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")

add_subdirectory("thirdparty")

add_subdirectory("serializer_lib")
add_subdirectory("serializer_tests")
add_subdirectory("takoyaki_lib")
add_subdirectory("takoyaki_tests")
add_subdirectory("takoyaki_app")