#ifndef LIBLOAD_H_INCLUDED
#define LIBLOAD_H_INCLUDED 0

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LIBLOAD_SUCCESS = 0,
    LIBLOAD_ERROR
} LIBLOAD_STATUS;

typedef struct {
    void *_handler;
} LibloadHandler;

typedef void (*LibloadSymbol)(void);

LIBLOAD_STATUS libloadfile (LibloadHandler *handler, char *editable_filepath);

LIBLOAD_STATUS libloadsymbol (
    LibloadHandler *handler, LibloadSymbol *symbol, char *symbol_name
);

LIBLOAD_STATUS libloadfinish (LibloadHandler *handler);

#ifdef __cplusplus
}
#endif

#endif
