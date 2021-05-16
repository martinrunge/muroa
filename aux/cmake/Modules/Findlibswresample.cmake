# - Try to find libswresample library
# Once done, this will define
#
#  libswresample_FOUND - system has libavformat libs
#  libswresample_INCLUDE_DIRS - the libavformat include directories
#  libswresample_LIBRARIES - link these to use libavformat

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libswresample_PKGCONF  QUIET libswresample)

find_path(libswresample_INCLUDE_DIR libswresample/swresample.h
          HINTS ${libswresample_PKGCONF_INCLUDEDIR} ${libswresample_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libswresample )

find_library(libswresample_LIBRARY NAMES libswresample swresample
             HINTS ${libswresample_PKGCONF__LIBDIR} ${libswresample_PKGCONF_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libswresample  DEFAULT_MSG
                                  libswresample_LIBRARY libswresample_INCLUDE_DIR)
                                  
# MESSAGE( libswresample: "${libswresample_LIBRARY}")

mark_as_advanced(libswresample_INCLUDE_DIR libswresample_LIBRARY )

