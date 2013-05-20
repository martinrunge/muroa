# - Try to find log4cplus libraries
# Once done, this will define
#
#  log4cplus_FOUND - system has log4cplus libs
#  log4cplus_INCLUDE_DIRS - the log4cplus include directories
#  log4cplus_LIBRARIES - link these to use log4cplus

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(log4cplus_PKGCONF  QUIET log4cplus)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(log4cplus_PKGCONF log4cplus)


find_path(log4cplus_INCLUDE_DIR log4cplus/logger.h
          HINTS ${log4cplus_PKGCONF_INCLUDEDIR} ${log4cplus_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES log4cplus )

find_library(log4cplus_LIBRARY NAMES log4cplus avutil
             HINTS ${log4cplus_PKGCONF__LIBDIR} ${log4cplus_PKGCONF_LIBRARY_DIRS} )

set(log4cplus_LIBRARIES ${log4cplus_LIBRARY} )
set(log4cplus_INCLUDE_DIRS ${log4cplus_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(log4cplus  DEFAULT_MSG
                                  log4cplus_LIBRARY log4cplus_INCLUDE_DIR)
                                  
# MESSAGE( log4cplus: "${log4cplus_LIBRARY}")

mark_as_advanced(log4cplus_INCLUDE_DIR log4cplus_LIBRARY )

