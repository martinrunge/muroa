cmake_minimum_required (VERSION 2.6) 
project ( muroa ) 

# Recurse into the subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (libdsaudio)  
add_subdirectory (libmuroadaemon)  
add_subdirectory (libstreamsrv)
add_subdirectory (mqtclient)
add_subdirectory (muroad)
# add_subdirectory (dsserver)
add_subdirectory (libmuroa++)
add_subdirectory (libsock++)
add_subdirectory (mmscanner)
add_subdirectory (msessiond)
