# - Try to find libsamplerate libraries
# Once done, this will define
#
#  libsamplerate_FOUND - system has libsamplerate libs
#  libsamplerate_INCLUDE_DIRS - the libsamplerate include directories
#  libsamplerate_LIBRARIES - link these to use libsamplerate

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libsamplerate_PKGCONF  QUIET libsamplerate)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libsamplerate_PKGCONF libsamplerate)


find_path(libsamplerate_INCLUDE_DIR samplerate.h
          HINTS ${libsamplerate_PKGCONF_INCLUDEDIR} ${libsamplerate_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libsamplerate )

find_library(libsamplerate_LIBRARY NAMES libsamplerate samplerate
             HINTS ${libsamplerate_PKGCONF__LIBDIR} ${libsamplerate_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libsamplerate  DEFAULT_MSG
                                  libsamplerate_LIBRARY libsamplerate_INCLUDE_DIR)
                                  
# MESSAGE( libsamplerate: "${libsamplerate_LIBRARY}")

mark_as_advanced(libsamplerate_INCLUDE_DIR libsamplerate_LIBRARY )

