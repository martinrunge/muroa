# - Try to find log4cplus libraries
# Once done, this will define
#
#  log4cplus_FOUND - system has log4cplus libs
#  log4cplus_DEFINITIONS - log4cplus #defines (especially if Logger::Logger(Logger&&) and Logger::operator=(Logger&&) 
#                          are included in the lib or if they need to be faked to be able to use C++11
#  log4cplus_INCLUDE_DIRS - the log4cplus include directories
#  log4cplus_LIBRARIES - link these to use log4cplus
#  log4cplus_VERSION   - version number
#  log4cplus_MAJOR_VERSION   - major version number

# include(LibFindMacros)
find_package( PkgConfig )

# Use pkg-config to get hints about paths
pkg_check_modules(log4cplus_PKGCONF  QUIET log4cplus)
# set(libavLIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})
# libfind_pkg_check_modules(log4cplus_PKGCONF log4cplus)


find_path(log4cplus_INCLUDE_DIR log4cplus/logger.h
          HINTS ${log4cplus_PKGCONF_INCLUDEDIR} ${log4cplus_PKGCONF_INCLUDE_DIRS}
          PATH_SUFFIXES log4cplus )

find_library(log4cplus_LIBRARY NAMES log4cplus
             HINTS ${log4cplus_PKGCONF__LIBDIR} ${log4cplus_PKGCONF_LIBRARY_DIRS} )

if(EXISTS "${log4cplus_INCLUDE_DIR}/log4cplus/version.h")
  file(STRINGS "${log4cplus_INCLUDE_DIR}/log4cplus/version.h"
    _log4cplus_version_string REGEX "#define LOG4CPLUS_VERSION LOG4CPLUS_MAKE_VERSION"
  )
  string(REGEX REPLACE "#define LOG4CPLUS_VERSION LOG4CPLUS_MAKE_VERSION\((.*)\).*$" "\\1"
    _log4cplus_version_string "${_log4cplus_version_string}"
  )
  string(REGEX REPLACE "[(]([0-9]+),.*[)].*$" "\\1"
    log4cplus_MAJOR_VERSION "${_log4cplus_version_string}"
  )
  string(REGEX REPLACE "[(].+, ([0-9]+),.+[)].*$" "\\1"
    log4cplus_MINOR_VERSION "${_log4cplus_version_string}"
  )
  string(REGEX REPLACE "[(].*,.*, ([0-9]+)[)].*$" "\\1"
    log4cplus_PATCH_VERSION "${_log4cplus_version_string}"
  )
  unset(_log4cplus_version_string)

  set(log4cplus_VERSION ${log4cplus_MAJOR_VERSION}.${log4cplus_MINOR_VERSION}.${log4cplus_PATCH_VERSION})
endif()


set(log4cplus_LIBRARIES ${log4cplus_LIBRARY} )
set(log4cplus_INCLUDE_DIRS ${log4cplus_INCLUDE_DIR} )
set(log4cplus_DEFINITIONS "-Dlog4cplus_MAJOR_VERSION=${log4cplus_MAJOR_VERSION}")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(log4cplus  DEFAULT_MSG
                                  log4cplus_LIBRARY log4cplus_INCLUDE_DIR)
                                  
MESSAGE( log4cplus: "${log4cplus_LIBRARIES}   ${log4cplus_INCLUDE_DIRS}   ${log4cplus_VERSION}")

set(CMAKE_REQUIRED_FLAGS "-std=c++0x -pthread")
set(CMAKE_REQUIRED_DEFINITIONS "")
# INCLUDE (CheckLog4cplusCXX11) 
# CHECK_LOG4CPLUS_CXX11(${log4cplus_LIBRARY}  "" ${LOG4CPLUS_HAS_RVAL_CTOR})

#MESSAGE( STATUS "LOG4CPLUS_HAS_RVAL_CTOR: ${LOG4CPLUS_HAS_RVAL_CTOR}")
#if( NOT LOG4CPLUS_HAS_RVAL_CTOR  )
#message( WARNING "${log4cplus_LIBRARY} was not compiled with C++11 support!
#  The following methods are defined in log4cplus headers if compiled with -std=c++11:
#  - Logger::Logger(Logger&&)
#  - Logger::operator=(Logger&&)
#  Faking them to avoid link errors!!! Rebuild liblog4cplus with 'CXXFLAGS=-std=c++11' to be clean. 
#")
#set( MUROA_FAKE_LOG4CPLUS_RVALREF  "yes") 
#set( log4cplus_DEFINITIONS "-DMUROA_FAKE_LOG4CPLUS_RVALREF")
#endif( NOT LOG4CPLUS_HAS_RVAL_CTOR )

mark_as_advanced(log4cplus_DEFINITIONS log4cplus_INCLUDE_DIR log4cplus_LIBRARY Log4cplus_VERSION)
                 
