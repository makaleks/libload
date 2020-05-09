sources = ['libload.c']

# https://mesonbuild.com/Reference-manual.html#host_machine-object
# https://mesonbuild.com/Reference-tables.html#operating-system-names
# C-literals to use string-like integer ids
if 'windows' == host_machine.system()
    PLATFORM_NAME = '\'WIN\''
    # VS2015 automatically links its libraries
    libload_dep   = dependency('', required: false)
    c_flags       = [
        # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category
        '/WX', '/W4',
    ]
else
    PLATFORM_NAME = '\'POSX\''
    libload_dep   = meson.get_compiler('c').find_library('dl')
    # Assume flags are GCC-compatible
    c_flags       = [
        # It is impossible to include all GCC warnings
        # https://stackoverflow.com/a/11720263
        '-Werror', '-Wall', '-Wextra', '-pedantic',
        # Used as convenient use if #ifdef
        '-Wno-multichar',
    ]
endif

conf_file = configure_file(
    output: 'buildtime_config.h',
    configuration: {
        'PLATFORM_NAME': PLATFORM_NAME,
    },
)
# https://mesonbuild.com/Configuration.html#a-full-example
extended_lib_include_dirs = [
    lib_include_dirs,
    # Use current build directory to find a build-time generated config.h file
    include_directories('.'),
]

libload_both = both_libraries(
  'libload',
  sources,
  dependencies:        libload_dep,
  include_directories: lib_include_dirs,
  # Project name already contains 'lib' prefix
  name_prefix:         '',
  c_args: c_flags,
#link_args: '/OPT:NOLBR',
)

libload_static = libload_both.get_static_lib()
libload_shared = libload_both.get_shared_lib()

libload_dep = declare_dependency(
  link_with: libload_both, include_directories: lib_include_dirs
)

# RU-only note: ninja и meson для русского вывода используют кодировку cp1251
# но visual studio cmd использует cp866 (выводится командой chcp)
# Починить пожно прогоном chcp1251
