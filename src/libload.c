#include "libload.h"
#include "buildtime_config.h"

#include <stddef.h>
#include <string.h>

#if PLATFORM_NAME == 'POSX'
// http://man7.org/linux/man-pages/man3/dlopen.3.html
#include <dlfcn.h>
#elif PLATFORM_NAME == 'WIN'
// https://docs.microsoft.com/windows/win32/dlls/using-run-time-dynamic-linking
// https://docs.microsoft.com/windows/win32/api/windef/
// /Wall causes errors in Windows.handler
// Using <libloaderapi.h> instead of Windows.h causes "No Target Architecture" error
#include <Windows.h>
#endif

LIBLOAD_STATUS libloadfile (LibloadHandler *handler, char *editable_filepath) {
    if (
        NULL == handler || NULL == editable_filepath 
        || '\0' == *editable_filepath
    ) {
        return LIBLOAD_ERROR;
    }
    void *raw_handler = NULL;
#if PLATFORM_NAME == 'POSX'
    raw_handler = dlopen(editable_filepath, RTLD_NOW);
#elif PLATFORM_NAME == 'WIN'
    // LoadLibrary automatically appends .dll
    size_t filepath_length  = strlen(editable_filepath);
    char   *filepath_suffix = editable_filepath + filepath_length - 3;
    char   suffix_backup    = '\0';
    if (
        filepath_suffix > editable_filepath
        && 0 == strcmp(".so", filepath_suffix)
    ) {
        suffix_backup      = *filepath_suffix;
        *filepath_suffix = '\0';
    }
	puts(editable_filepath);
    raw_handler = LoadLibrary(editable_filepath);
    if ('\0' != suffix_backup) {
        *filepath_suffix = suffix_backup;
    }
#endif
    if (NULL == raw_handler) {
        //unsigned long err = GetLastError();
        //printf("Error: %lu\n", err);
        return LIBLOAD_ERROR;
    }

    memset(handler, 0, sizeof(*handler));
    handler->_handler = raw_handler;
    return LIBLOAD_SUCCESS;
}


LIBLOAD_STATUS libloadsymbol (
    LibloadHandler *handler, LibloadSymbol *symbol, char *symbol_name
) {
    if (
        NULL == handler || NULL == symbol 
        || NULL == symbol_name || '\0' == *symbol_name
    ) {
        return LIBLOAD_ERROR;
    }
#if PLATFORM_NAME == 'POSX'
    LibloadSymbol raw_symbol = NULL;
    // ISO C doesn't actually *forbids* the conversion void=>void(*)(),
    // but POSIX actually *allows* it
    // https://stackoverflow.com/a/31528674
    *(void**)&raw_symbol = dlsym(handler->_handler, symbol_name);
#elif PLATFORM_NAME == 'WIN'
    int (__stdcall *raw_symbol)() = NULL;
    raw_symbol = GetProcAddress(handler->_handler, symbol_name);
#endif
    // TODO: compare & use errors from dlerror(), etc.
    if (NULL == raw_symbol) {
        //unsigned long err = GetLastError();
        //printf("Error: %lu\n", err);
        return LIBLOAD_ERROR;
    }
    else {
        *symbol = (LibloadSymbol)raw_symbol;
        return LIBLOAD_SUCCESS;
    }
}

LIBLOAD_STATUS libloadfinish (LibloadHandler *handler) {
    if (NULL == handler) {
        return LIBLOAD_ERROR;
    }
    int raw_status = 0;
#if PLATFORM_NAME == 'POSX'
    raw_status = dlclose(handler->_handler);
#elif PLATFORM_NAME == 'WIN'
    raw_status = !FreeLibrary(handler->_handler);
#endif
    if (0 != raw_status)
        return LIBLOAD_ERROR;

    memset(handler, 0, sizeof(*handler));
    return LIBLOAD_SUCCESS;
}
