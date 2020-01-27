#include "test_shared.h"

#include <stddef.h> // NULL
#include <ctype.h>  // toupper()

// Compiler will help in case of mismatch with .h-file
char* test_shared (char *str) {
    if (NULL == str) {
        // Data-provided
        return "Hello, plugin!";
    }
    else {
        // Data-supplied
        char *cur = str;
        while ('\0' != *cur) {
            *cur = toupper(*cur);
            cur++;
        }
        return str;
    }
}

