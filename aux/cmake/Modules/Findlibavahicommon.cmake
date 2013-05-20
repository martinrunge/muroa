# - Try to find libavahicommon library
# Once done, this will define
#
#  libavahicommon_FOUND - system has libavformat libs
#  libavahicommon_INCLUDE_DIRS - the libavformat include directories
#  libavahicommon_LIBRARY - link these to use libavformat

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavahicommon_PKGCONF  QUIET libavahicommon)

# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavahicommon_PKGCONF libavahicommon)


find_path(libavahicommon_INCLUDE_DIR avahi-common/thread-watch.h
          HINTS ${libavahicommon_PKGCONF_INCLUDEDIR} ${libavahicommon_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavahicommon )

find_library(libavahicommon_LIBRARY NAMES libavahi-common avahi-common
             HINTS ${libavahicommon_PKGCONF__LIBDIR} ${libavahicommon_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavahicommon  DEFAULT_MSG
                                  libavahicommon_LIBRARY libavahicommon_INCLUDE_DIR)
                                  
# MESSAGE( libavahicommon: "${libavahicommon_LIBRARY}")

mark_as_advanced(libavformat_INCLUDE_DIR libavahicommon_LIBRARY )

