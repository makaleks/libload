# Allows `dlopen` to search in caller directory
set(CMAKE_BUILD_RPATH $ORIGIN)

add_executable(overview "overview.c")

target_link_libraries(overview libload_static)
#set_target_properties(overview PROPERTIES BUILD_RPATH_USE_ORIGIN TRUE)

add_test(NAME overview-test COMMAND overview WORKING_DIRECTORY $<TARGET_FILE_DIR:overview>)
