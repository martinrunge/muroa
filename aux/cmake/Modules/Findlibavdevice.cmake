# - Try to find libavdevice libraries
# Once done, this will define
#
#  libavdevice_FOUND - system has libavdevice libs
#  libavdevice_INCLUDE_DIRS - the libavdevice include directories
#  libavdevice_LIBRARIES - link these to use libavdevice

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavdevice_PKGCONF  QUIET libavdevice)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavdevice_PKGCONF libavdevice)


find_path(libavdevice_INCLUDE_DIR libavdevice/avdevice.h
          HINTS ${libavdevice_PKGCONF_INCLUDEDIR} ${libavdevice_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavdevice )

find_library(libavdevice_LIBRARY NAMES libavdevice avdevice
             HINTS ${libavdevice_PKGCONF__LIBDIR} ${libavdevice_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavdevice  DEFAULT_MSG
                                  libavdevice_LIBRARY libavdevice_INCLUDE_DIR)
                                  
# MESSAGE( libavdevice: "${libavdevice_LIBRARY}")

mark_as_advanced(libavdevice_INCLUDE_DIR libavdevice_LIBRARY )

