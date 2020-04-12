project('libload', 'c', default_options: [
    # Make 'library()' similar to 'static_library'
    #'default_library=static',
	# Saves all targets in single directory 'meson-out'
	'layout=flat',
])

lib_include_dirs = include_directories('include')


subdir('include')
subdir('src')

# Avoid file collisions with callee project with `meson-out` flat layout
if not meson.is_subproject()
  subdir('tests')
endif


