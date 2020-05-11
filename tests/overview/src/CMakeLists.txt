add_library(test-shared MODULE "test-shared.c")

set_target_properties(test-shared PROPERTIES PREFIX "")
target_compile_definitions(test-shared PRIVATE -DTEST_SHARED_BUILD)
