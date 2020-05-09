rpath_value = '$ORIGIN'

overview = executable(
  'overview', 
  'overview.c',
  include_directories: test_include_dirs,
  link_with: libload_static,
  build_rpath: rpath_value,
  #link_args: '/OPT:NOLBR',
)

test('overview', overview)

