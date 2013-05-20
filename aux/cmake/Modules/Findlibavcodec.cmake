# - Try to find libavcodec libraries
# Once done, this will define
#
#  libavcodec_FOUND - system has libavcodec libs
#  libavcodec_INCLUDE_DIRS - the libavcodec include directories
#  libavcodec_LIBRARIES - link these to use libavcodec

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavcodec_PKGCONF  QUIET libavcodec)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavcodec_PKGCONF libavcodec)


find_path(libavcodec_INCLUDE_DIR libavcodec/avcodec.h
          HINTS ${libavcodec_PKGCONF_INCLUDEDIR} ${libavcodec_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavcodec )

find_library(libavcodec_LIBRARY NAMES libavcodec avcodec
             HINTS ${libavcodec_PKGCONF__LIBDIR} ${libavcodec_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavcodec  DEFAULT_MSG
                                  libavcodec_LIBRARY libavcodec_INCLUDE_DIR)
                                  
# MESSAGE( libavcodec: "${libavcodec_LIBRARY}")

mark_as_advanced(libavcodec_INCLUDE_DIR libavcodec_LIBRARY )

