cmake_minimum_required(VERSION 3.12)
project("libload" LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("scripts/libload_get_project_targets.cmake")
_libload_get_project_layout_final_constants()
set(include_dir ${_LIBLOAD_INCLUDE_DIRECTORY_RELATIVE_PATH}) # "include"

include_directories("${include_dir}")

include("output_layout.cmake")

add_subdirectory("include")
add_subdirectory("src")

enable_testing()
add_subdirectory("tests")
