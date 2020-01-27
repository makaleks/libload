#include <stdio.h>

#include <test_shared.h>
#include <libload.h>

int main () {
    puts("Example of libload started\n");

    LIBLOAD_STATUS   status   = LIBLOAD_SUCCESS;
    function_str_str func_ptr = NULL;
    LibloadHandler   handler;
    // 'test-shared.so' in Unix, 'test-shared' in Windows
    char library_path [] = "test-shared.so";

    // Meson note: user ../src/test-shared for default --layout
    // and test-shared for --layout=flat
    status = libloadfile(&handler, library_path);
    if (LIBLOAD_SUCCESS != status) {
        puts("Error on file load!");
        return -1;
    }
    puts("[done]: library loaded");

    status = libloadsymbol(
        &handler, (LibloadSymbol*)&func_ptr, "test_shared"
    );
    if (LIBLOAD_SUCCESS != status) {
        puts("Error on symbol load!");
        libloadfinish(&handler);
        return -1;
    }
    puts("[done]: requested function was found in library");

    puts("");

    char test_str[] = "Test string";
    char *test_ptr  = NULL;

    test_ptr = func_ptr(NULL);
    printf("[library output 1]: %s\n", test_ptr);

    test_ptr = func_ptr(test_str);
    printf("[library output 2]: %s\n", test_ptr);

    puts("");

    puts("[done]: code from library has run");

    status = libloadfinish(&handler);
    if (LIBLOAD_SUCCESS != status) {
        puts("Error on symbol unload!");
        return -1;
    }
    puts("[done]: library unloaded");

    puts("\nExample of libload finished!");
    return 0;
}
