message("-- Fetching External Project: stb")

FetchContent_Declare(
  stb
  GIT_REPOSITORY    https://github.com/nothings/stb
  GIT_TAG           80c8f6af0304588b9d780a41015472013b705194
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  SOURCE_DIR      "${FETCHCONTENT_BASE_DIR}/stb/stb"
)

FetchContent_GetProperties(stb)
if (NOT stb)
  FetchContent_Populate(stb)

  add_library(stb INTERFACE)
  target_include_directories(stb INTERFACE "${stb_SOURCE_DIR}/..")
endif()