# - Try to find libavformat libraries
# Once done, this will define
#
#  libavformat_FOUND - system has libavformat libs
#  libavformat_INCLUDE_DIRS - the libavformat include directories
#  libavformat_LIBRARIES - link these to use libavformat

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavformat_PKGCONF  QUIET libavformat)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavformat_PKGCONF libavformat)


find_path(libavformat_INCLUDE_DIR libavformat/avformat.h
          HINTS ${libavformat_PKGCONF_INCLUDEDIR} ${libavformat_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavformat )

find_library(libavformat_LIBRARY NAMES libavformat avformat
             HINTS ${libavformat_PKGCONF__LIBDIR} ${libavformat_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavformat  DEFAULT_MSG
                                  libavformat_LIBRARY libavformat_INCLUDE_DIR)
                                  
# MESSAGE( libavformat: "${libavformat_LIBRARY}")

mark_as_advanced(libavformat_INCLUDE_DIR libavformat_LIBRARY )

