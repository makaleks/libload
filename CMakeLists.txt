cmake_minimum_required(VERSION 3.12)

project("libload" LANGUAGES C)

include_directories("include")

include("output_layout.cmake")

add_subdirectory("include")
add_subdirectory("src")

enable_testing()
add_subdirectory("tests")
