# libload

This simple C library provides cross-platform interface to load dynamic (aka
'shared') libraries in runtime. Flags are currently optimized for loading
local libraries, making it possible to be used in plugin development (C ABI
is the most popular plugin binary interface).

Currently runs on Windows and Linux. Linux interface is supposed to be
POSIX-compatible. Compilation flags are strict.

## Example

An example can be found at `tests/overview`. It is an example of project
with a library, loaded at runtime.

To be short, the library only provides functions:
- `libloadfile` - loads library file in runtime.
- `libloadsymbol` - loads function or global variable.
- `libloadfinish` - terminates the library.

## Build

This project uses CMake build system:

```sh
cmake -B build
cmake --build build
cd build && ctest
```

The build output can be found at `build/libload/`

## Notes

### Why do library must have `.so` extension?
Windows is able to append `.dll` automatically, `dl` is unable. Also that is
why library has to be allocated in array, not as just string literal.

### Why Windows flags do not contain `/Wall` option?
`Windows.h` does not meet `/Wall` requirements. But if I use 
`libloaderapi.h` instead of `Windows.h`, I get `No target architecture` 
error. If you know the solution, I am ready to integrate it.

