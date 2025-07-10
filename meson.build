project('gst-xevd-parser', 'c',
  version : '1.0.0',
  license : 'LGPL',
  meson_version : '>= 0.53.0',
  default_options : ['c_std=gnu11'])

# Initialisation du compilateur
cc = meson.get_compiler('c')

# Dependencies
gst_dep = dependency('gstreamer-1.0', version : '>=1.18.0')
gstvideo_dep = dependency('gstreamer-video-1.0')

# Chemins de recherche standard
xevd_lib_dirs = [
  '/usr/local/lib',
  '/usr/lib/x86_64-linux-gnu', 
  '/usr/lib',
  # Ajoutez d'autres chemins standard si nécessaire
]

xevd_inc_dirs = [
  '/usr/local/include',
  '/usr/include',
  '/usr/local/include/xevd'

]

# Recherche de la bibliothèque XEVE
xevd_dep = cc.find_library('xevd',
  required : true,
  dirs : xevd_lib_dirs)

# Configuration des includes
xevd_inc = include_directories(xevd_inc_dirs)

mathlib = cc.find_library('m', required: true)

# Vérification du header
if not cc.has_header('xevd.h')
  error('xevd.h header not found. Please install libxevd-dev or check installation path')
endif

# Plugin config
plugin_sources = [
  'gstxevdparser.c',
]

c_args = [
  '-DHAVE_CONFIG_H',
  '-DGST_USE_UNSTABLE_API',
  '-DG_LOG_DOMAIN="GST-XEVD"'
]

shared_library('gstxevdparser',
  plugin_sources,
  c_args : c_args,
  dependencies : [gst_dep, gstvideo_dep, xevd_dep, mathlib],
  include_directories : xevd_inc,
  install : true,
  install_dir : join_paths(get_option('libdir'), 'gstreamer-1.0'),
  name_prefix : '',
)
