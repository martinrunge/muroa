cmake_minimum_required (VERSION 2.8.3) 
project ( muroa ) 

# Recurse into the subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 

option( msessiond "build the muroa session daemon"       "yes")
option( muroad "build the muroa audio streaming daemon"  "yes")
option( mqtclient "build the Qt based GUI client"        "yes")


if( ${msessiond} )
  add_subdirectory (3rdparty/libxdiff-0.23)
  add_subdirectory (libstreamsrv)
  add_subdirectory (mmscanner)
  add_subdirectory (msessiond)
endif( ${msessiond} )

if( ${muroad} )
  add_subdirectory (libdsaudio)  
  add_subdirectory (muroad)
endif( ${muroad} )

if( ${mqtclient} )
  add_subdirectory (mqtclient)
endif( ${mqtclient} )


if( ${msessiond} OR ${mqtclient} )
  add_subdirectory (libmuroa++)
endif( ${msessiond} OR ${mqtclient} )


if( ${msessiond} OR  ${muroad} )
  add_subdirectory (libmuroadaemon)  
  add_subdirectory (libsock++)
endif( ${msessiond} OR ${muroad} )
