project('rectselect', 'cpp',
  version: '0.1',
  default_options: [
    'warning_level=3',
    'cpp_std=c++20',
  ]
)

source_files = [
  'src/main.cpp',
  'src/ImageDrawingArea.cpp',
  'src/RectSelect.cpp',
]

executable(
  'rectselect',
  source_files,
  dependencies: [dependency('gtkmm-3.0')],
  install: true,
)

