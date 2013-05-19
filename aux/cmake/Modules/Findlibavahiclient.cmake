# - Try to find libavahiclient library
# Once done, this will define
#
#  libavahiclient_FOUND - system has libavformat libs
#  libavahiclient_INCLUDE_DIRS - the libavformat include directories
#  libavahiclient_LIBRARY - link these to use libavformat

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(libavahiclient_PKGCONF  QUIET libavahiclient)

# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(libavahiclient_PKGCONF libavahiclient)


find_path(libavahiclient_INCLUDE_DIR avahi-client/client.h
          HINTS ${libavahiclient_PKGCONF_INCLUDEDIR} ${libavahiclient_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES libavahiclient )

find_library(libavahiclient_LIBRARY NAMES libavahi-client avahi-client
             HINTS ${libavahiclient_PKGCONF__LIBDIR} ${libavahiclient_PKGCONF_LIBRARY_DIRS} )

# set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
# set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libavahiclient  DEFAULT_MSG
                                  libavahiclient_LIBRARY libavahiclient_INCLUDE_DIR)
                                  
MESSAGE( libavahiclient: "${libavahiclient_LIBRARY}")

mark_as_advanced(libavformat_INCLUDE_DIR libavahiclient_LIBRARY )

