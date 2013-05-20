# - Try to find libavutil libraries
# Once done, this will define
#
#  libavutil_FOUND - system has libavutil libs
#  libavutil_INCLUDE_DIRS - the libavutil include directories
#  libavutil_LIBRARIES - link these to use libavutil

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavutil_PKGCONF  QUIET libavutil)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavutil_PKGCONF libavutil)


find_path(libavutil_INCLUDE_DIR libavutil/avutil.h
          HINTS ${libavutil_PKGCONF_INCLUDEDIR} ${libavutil_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavutil )

find_library(libavutil_LIBRARY NAMES libavutil avutil
             HINTS ${libavutil_PKGCONF__LIBDIR} ${libavutil_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavutil  DEFAULT_MSG
                                  libavutil_LIBRARY libavutil_INCLUDE_DIR)
                                  
# MESSAGE( libavutil: "${libavutil_LIBRARY}")

mark_as_advanced(libavutil_INCLUDE_DIR libavutil_LIBRARY )

