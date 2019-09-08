include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         3.3
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)


FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()
#FetchContent_MakeAvailable(glfw)

set_target_properties(glfw PROPERTIES FOLDER "thirdparty")