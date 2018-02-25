project('libscrypt', 'c', version : '0.1.21')

version = meson.project_version()

sources = ['crypto_scrypt-check.c',
'crypto_scrypt-hash.c',
'crypto_scrypt-hexconvert.c',
'crypto_scrypt-nosse.c',
'crypto-mcf.c',
'crypto-scrypt-saltgen.c',
'slowequals.c',
'sha256.c',
'b64.c'
]
inc = include_directories('./')
install_headers('libscrypt.h','sha256.h',subdir: 'libscrypt')
lib = library('libscrypt', sources: sources, version: version,
include_directories : inc,
c_args: ['-Wall', '-O2', '-ffast-math', '-g', '-D_FORTIFY_SOURCE=2', '-fstack-protector'],
install: true)

libscrypt_dep = declare_dependency(include_directories : inc,
  link_with : lib)
