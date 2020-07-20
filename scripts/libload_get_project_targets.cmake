# https://cmake.org/cmake/help/latest/command/cmake_parse_arguments.html

function (_libload_get_project_layout_final_constants)
    set(arg_options     "")
    set(
        arg_values
        "CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH"
        "CHECK_STATIC_LIBRARY_RELATIVE_PATH"
        "CHECK_SHARED_LIBRARY_RELATIVE_PATH"
    )
    set(arg_multivalues "")
    cmake_parse_arguments(
        LIBLOAD "${arg_options}" "${arg_values}" "${arg_multivalues}" ${ARGN}
    )

    set(
        include_directory_relative_path
        "include"
    )
    if (
        DEFINED LIBLOAD_CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH
        AND NOT "${LIBLOAD_CHECK_INCLUDE_DIRECTORY_RELATIVE_PATH}"
            STREQUAL "${include_directory_relative_path}"
    )
        message(
            FATAL_ERROR "Include directory path settings are not concordant."
        )
    endif()
    set(
        _LIBLOAD_INCLUDE_DIRECTORY_RELATIVE_PATH
        ${include_directory_relative_path}
        PARENT_SCOPE
    )

    set(
        static_library_relative_path
        "libload/lib/libload${CMAKE_STATIC_LIBRARY_SUFFIX}"
    )
    if (
        DEFINED LIBLOAD_CHECK_STATIC_LIBRARY_RELATIVE_PATH
        AND NOT "${LIBLOAD_CHECK_STATIC_LIBRARY_RELATIVE_PATH}"
            STREQUAL "${static_library_relative_path}"
    )
        message(
            FATAL_ERROR "Static library path settings are not concordant."
            "\nGot ${LIBLOAD_CHECK_STATIC_LIBRARY_RELATIVE_PATH}"
            "\nNed ${static_library_relative_path}"
        )
    endif()
    set(
        _LIBLOAD_STATIC_LIBRARY_RELATIVE_PATH
        ${static_library_relative_path}
        PARENT_SCOPE
    )

    set(
        shared_library_relative_path
        "libload/lib/libload${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
    if (
        DEFINED LIBLOAD_CHECK_SHARED_LIBRARY_RELATIVE_PATH
        AND NOT "${LIBLOAD_CHECK_SHARED_LIBRARY_RELATIVE_PATH}"
            STREQUAL "${shared_library_relative_path}"
    )
        message(
            FATAL_ERROR "Shared library path settings are not concordant."
        )
    endif()
    set(
        _LIBLOAD_SHARED_LIBRARY_RELATIVE_PATH
        ${shared_library_relative_path}
        PARENT_SCOPE
    )

    set(
        _LIBLOAD_LIBRARY_TARGET_LINK_DEPENDENCIES ${CMAKE_DL_LIBS} PARENT_SCOPE
    )
endfunction ()

function (libload_get_project_targets)
    set(arg_options     "")
    set(arg_values      "SOURCE_DIR" "CUSTOM_PREFIX")
    set(arg_multivalues "")
    cmake_parse_arguments(
        LIBLOAD "${arg_options}" "${arg_values}" "${arg_multivalues}" ${ARGN}
    )

    if (
        DEFINED ENV{OVERRIDE_THIRD_PARTY_SOURCE_DIR}
        AND DEFINED ENV{OVERRIDE_THIRD_PARTY_BINARY_DIR}
    )
        set(LIBLOAD_SOURCE_DIR $ENV{OVERRIDE_THIRD_PARTY_SOURCE_DIR})
        set(LIBLOAD_BINARY_DIR $ENV{OVERRIDE_THIRD_PARTY_BINARY_DIR})
        set(
            CMD
"${CMAKE_COMMAND} -E env \
OVERRIDE_THIRD_PARTY_SOURCE_DIR=$ENV{OVERRIDE_THIRD_PARTY_SOURCE_DIR} \
OVERRIDE_THIRD_PARTY_BINARY_DIR=$ENV{OVERRIDE_THIRD_PARTY_BINARY_DIR} \
${CMAKE_COMMAND}"
        )
    else ()
        set(CMD "${CMAKE_COMMAND}")
    endif()

    ExternalProject_Add(libload_dependency
        CMAKE_COMMAND   "${CMD}"
        SOURCE_DIR      "${LIBLOAD_SOURCE_DIR}"
        INSTALL_COMMAND ""
        PREFIX          "libload_dependency"
        BINARY_DIR      ${LIBLOAD_BINARY_DIR}
    )
    ExternalProject_Get_Property(libload_dependency SOURCE_DIR BINARY_DIR)

    _libload_get_project_layout_final_constants()

    set(
        "${CUSTOM_PREFIX}libload_include_directory"
        "${SOURCE_DIR}/${_LIBLOAD_INCLUDE_DIRECTORY_RELATIVE_PATH}"
        PARENT_SCOPE
    )

    set(libload_static_target_name "${CUSTOM_PREFIX}libload_static")
    set(libload_shared_target_name "${CUSTOM_PREFIX}libload_shared")

    add_library(${libload_static_target_name} STATIC IMPORTED GLOBAL)
    add_library(${libload_shared_target_name} SHARED IMPORTED GLOBAL)

    set_target_properties(
        ${libload_static_target_name} PROPERTIES IMPORTED_LOCATION
        "${BINARY_DIR}/${_LIBLOAD_STATIC_LIBRARY_RELATIVE_PATH}"
    )
    set_target_properties(
        ${libload_shared_target_name} PROPERTIES IMPORTED_LOCATION
        "${BINARY_DIR}/${_LIBLOAD_SHARED_LIBRARY_RELATIVE_PATH}"
    )

    target_link_libraries(
        ${libload_static_target_name}
        INTERFACE
        ${_LIBLOAD_LIBRARY_TARGET_LINK_DEPENDENCIES}
    )
    target_link_libraries(
        ${libload_shared_target_name}
        INTERFACE
        ${_LIBLOAD_LIBRARY_TARGET_LINK_DEPENDENCIES}
    )

    set(
        ${libload_static_target_name} ${libload_static_target_name} PARENT_SCOPE
    )
    set(
        ${libload_shared_target_name} ${libload_static_target_name} PARENT_SCOPE
    )
endfunction ()
