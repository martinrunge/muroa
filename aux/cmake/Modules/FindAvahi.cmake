#
# Try to find Avahi client side libraries:
#
#   - avahi-client
#   - avahi-common
#   - avahi-qt4
#
#  find_package(Avahi COMPONENTS client common qt4)
# 
# Once done, this will define
#
#  Avahi_FOUND - system has libavformat libs
#  Avahi_INCLUDE_DIRS - the libavformat include directories
#  Avahi_LIBRARIES - link these to use libavformat
#

# Use find_package( Avahi COMPONENTS ... ) to enable modules
if( Avahi_FIND_COMPONENTS )
  foreach( component ${Avahi_FIND_COMPONENTS} )
    string( TOUPPER ${component} _COMPONENT )
    set( AVAHI_USE_${_COMPONENT} 1 )
  endforeach()
endif( Avahi_FIND_COMPONENTS )

include(FindPackageHandleStandardArgs)

find_package( PkgConfig )
if(PKG_CONFIG_FOUND)

  if(AVAHI_USE_COMMON)
    # Use pkg-config to get hints about paths
    pkg_check_modules(libavahicommon_PKGCONF  QUIET libavahicommon)

    find_path(libavahicommon_INCLUDE_DIR avahi-common/thread-watch.h
              HINTS ${libavahicommon_PKGCONF_INCLUDEDIR} ${libavahicommon_PKGCONF_INCLUDE_DIRS}
              PATH_SUFFIXES libavahicommon )

    find_library(libavahicommon_LIBRARY NAMES libavahi-common avahi-common
                 HINTS ${libavahicommon_PKGCONF__LIBDIR} ${libavahicommon_PKGCONF_LIBRARY_DIRS} )

    mark_as_advanced(libavahicommon_INCLUDE_DIR libavahicommon_LIBRARY )

    # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(Avahi  DEFAULT_MSG libavahicommon_LIBRARY libavahicommon_INCLUDE_DIR)
        
  endif(AVAHI_USE_COMMON)
    
  if(AVAHI_USE_CLIENT)
    # Use pkg-config to get hints about paths
    pkg_check_modules(libavahiclient_PKGCONF  QUIET libavahiclient)

    find_path(libavahiclient_INCLUDE_DIR avahi-client/client.h
              HINTS ${libavahiclient_PKGCONF_INCLUDEDIR} ${libavahiclient_PKGCONF_INCLUDE_DIRS}
              PATH_SUFFIXES libavahiclient )

    find_library(libavahiclient_LIBRARY NAMES libavahi-client avahi-client
                 HINTS ${libavahiclient_PKGCONF__LIBDIR} ${libavahiclient_PKGCONF_LIBRARY_DIRS} )

    mark_as_advanced(libavahiclient_INCLUDE_DIR libavahiclient_LIBRARY )

    # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(Avahi  DEFAULT_MSG libavahiclient_LIBRARY libavahiclient_INCLUDE_DIR)
                 
  endif(AVAHI_USE_CLIENT)

  if(AVAHI_USE_QT4)
    pkg_check_modules(libavahiqt4_PKGCONF  QUIET libavahiqt4)

    find_path(libavahiqt4_INCLUDE_DIR avahi-qt4/qt-watch.h
              HINTS ${libavahiqt4_PKGCONF_INCLUDEDIR} ${libavahiqt4_PKGCONF_INCLUDE_DIRS}
              PATH_SUFFIXES libavahiqt4 )
              
    message(STATUS "libavahiqt4_INCLUDE_DIR: $libavahiqt4_INCLUDE_DIR")

    find_library(libavahiqt4_LIBRARY NAMES libavahi-qt4 avahi-qt4
                 HINTS ${libavahiqt4_PKGCONF__LIBDIR} ${libavahiqt4_PKGCONF_LIBRARY_DIRS} )

    message(STATUS "libavahiqt4_LIBRARY: $libavahiqt4_LIBRARY")
          
    mark_as_advanced(libavahiqt4_INCLUDE_DIR libavahiqt4_LIBRARY )
  
    # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(Avahi DEFAULT_MSG libavahiqt4_LIBRARY libavahiqt4_INCLUDE_DIR )
    
                 
  endif(AVAHI_USE_QT4)
    
endif(PKG_CONFIG_FOUND)


set(Avahi_LIBRARIES ${libavahicommon_LIBRARY} ${libavahiclient_LIBRARY} ${libavahiqt4_LIBRARY} )
set(Avahi_INCLUDE_DIRS ${libavahicommon_INCLUDE_DIR} ${libavahiclient_INCLUDE_DIR} ${libavahiqt4_INCLUDE_DIR} )

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Avahi  DEFAULT_MSG Avahi_LIBRARIES Avahi_INCLUDE_DIRS )

                                  
# MESSAGE( libavahiclient: "${libavahiclient_LIBRARY}")


