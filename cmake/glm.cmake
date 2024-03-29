message("-- External Project: glm")
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY  https://github.com/g-truc/glm.git
	GIT_TAG         0.9.9.6
)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
	add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

set_target_properties(glm_static PROPERTIES FOLDER "thirdparty")

target_include_directories(glm_static PUBLIC ${glm_SOURCE_DIR})
