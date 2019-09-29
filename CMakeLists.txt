cmake_minimum_required(VERSION 3.12.4)
project("takoyaki" C CXX)

message("-- CONFIGURATION TYPES: ${CMAKE_CONFIGURATION_TYPES}")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")
include("cmake/cotire.cmake")
include("cmake/googletest.cmake")
include("cmake/glfw.cmake")
include("cmake/glm.cmake")
include("cmake/thirdparty.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")
set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("serializer_lib")
add_subdirectory("serializer_tests")
add_subdirectory("takoyaki_lib")
add_subdirectory("takoyaki_tests")
add_subdirectory("takoyaki_app")