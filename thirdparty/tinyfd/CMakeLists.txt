set(tinyfd_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB tinyfd_sources ${tinyfd_include_dir}/*.c)
file(GLOB tinyfd_headers ${tinyfd_include_dir}/*.h)

add_library(tinyfd STATIC ${tinyfd_sources} ${tinyfd_headers})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(tinyfd PUBLIC ${tinyfd_include_dir})

target_link_libraries(tinyfd glfw glad)
set_target_properties(tinyfd PROPERTIES FOLDER "thirdparty")