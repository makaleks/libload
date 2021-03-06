set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib")

list(APPEND sources "libload.c")

add_library(libload_objects OBJECT ${sources})

if (WIN32)
    set(PLATFORM_NAME "'WIN'")
    # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category
    list(APPEND c_flags
        "/WX" "/W4"
    )
else()
    set(PLATFORM_NAME "'POSX'")
    target_link_libraries(libload_objects PUBLIC ${CMAKE_DL_LIBS})
    list(APPEND c_flags
        # It is impossible to include all GCC warnings
        # https://stackoverflow.com/a/11720263
        "-Werror" "-Wall" "-Wextra" "-pedantic"
        # Used as convenient use if #ifdef
        "-Wno-multichar"
    )
endif()

target_compile_options(libload_objects PRIVATE ${c_flags})

configure_file("buildtime_config.h.in" "buildtime_config.h" @ONLY)
target_include_directories(libload_objects PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set(libload_static_target_name "libload_static")
set(libload_shared_target_name "libload_shared")

add_library(
    ${libload_static_target_name} STATIC $<TARGET_OBJECTS:libload_objects>
)
add_library(
    ${libload_shared_target_name} SHARED $<TARGET_OBJECTS:libload_objects>
)
# Need to copy list of libraries to link as it is not propagated with
# TARGET_OBJECTS
target_link_libraries(
    ${libload_static_target_name}
    $<TARGET_PROPERTY:libload_objects,LINK_LIBRARIES>
)
target_link_libraries(
    ${libload_shared_target_name}
    $<TARGET_PROPERTY:libload_objects,LINK_LIBRARIES>
)

# Project name already contains 'lib' prefix
set_target_properties(
    ${libload_static_target_name} ${libload_shared_target_name}
    PROPERTIES
    PREFIX ""
    OUTPUT_NAME "libload"
)

file(
    RELATIVE_PATH STATIC_LIB_RELATIVE_PATH
    ${PROJECT_BINARY_DIR}
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libload${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
file(
    RELATIVE_PATH SHARED_LIB_RELATIVE_PATH
    ${PROJECT_BINARY_DIR}
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libload${CMAKE_SHARED_LIBRARY_SUFFIX}"
)
_libload_get_project_layout_final_constants(
    CHECK_STATIC_LIBRARY_RELATIVE_PATH ${STATIC_LIB_RELATIVE_PATH}
    CHECK_SHARED_LIBRARY_RELATIVE_PATH ${SHARED_LIB_RELATIVE_PATH}
)
