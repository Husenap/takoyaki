message("-- External Project: googletest")
include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.8.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

enable_testing()
include(GoogleTest)

set(GTEST_FOLDER "thirdparty//googletest")
set_target_properties(gtest PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gmock PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gtest_main PROPERTIES FOLDER ${GTEST_FOLDER})
set_target_properties(gmock_main PROPERTIES FOLDER ${GTEST_FOLDER})