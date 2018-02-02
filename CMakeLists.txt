cmake_minimum_required(VERSION 3.0)
project(libscrypt LANGUAGES C)

set(
    sources
    b64.c b64.h
    crypto_scrypt-check.c
    crypto_scrypt-hash.c
    crypto_scrypt-hexconvert.c crypto_scrypt-hexconvert.h
    crypto_scrypt-nosse.c
    crypto-mcf.c
    crypto-scrypt-saltgen.c
    libscrypt.h
    main.c
    sha256.c sha256.h
    slowequals.c slowequals.h
    sysendian.h
)

add_library(scrypt ${sources})
target_compile_definitions(scrypt PRIVATE _FORTIFY_SOURCE=2)
set_target_properties(
    scrypt PROPERTIES
    PUBLIC_HEADER "libscrypt.h"
    SOVERSION 0
)

include(GNUInstallDirs)
install(
    TARGETS scrypt
    EXPORT libscryptTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION  "${CMAKE_INSTALL_INCLUDEDIR}"
)

include(CMakePackageConfigHelpers)
set(config_install_destination ${CMAKE_INSTALL_LIBDIR}/libscrypt/cmake)
configure_package_config_file(
    libscryptConfig.cmake.in libscryptConfig.cmake
    INSTALL_DESTINATION ${config_install_destination}
)
install(
    EXPORT libscryptTargets
    DESTINATION ${config_install_destination}
    NAMESPACE libscrypt::
)
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libscryptConfig.cmake
    DESTINATION ${config_install_destination}
)
