cmake_minimum_required (VERSION 2.6)

project (muroadaemon)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS system  REQUIRED )
find_package( EXPAT REQUIRED )
find_package( libavahiclient REQUIRED )
find_package( libavahicommon REQUIRED )


add_definitions(  )


set( SRC           CTcpConnection.cpp
                   CTcpServer.cpp
                   # CConnectionManager.cpp
                   CSignalHandler.cpp
                   CSettings.cpp
                   Exceptions.cpp
                   CApp.cpp
                   # main.cpp
                   # avahi/asio-watch.cpp
                   avahi/CDnsSdBase.cpp
                   avahi/CDnsSdAvahi.cpp
                   avahi/CServiceDesc.cpp
                   )
                  

set( HEADERS       CTcpConnection.h
                   CTcpServer.h
                   IConnectionManager.h
                   CSignalHandler.h
                   CSettings.h
                   Exceptions.h
                   CApp.h
                   # avahi/asio-watch.h
                   avahi/CDnsSdBase.h
                   avahi/CDnsSdAvahi.h
                   avahi/CServiceDesc.h
                   )
                   
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS} 
                                        ${EXPAT_INCLUDE_DIRS}
                                        ${log4cplus_INCLUDE_DIRS}
                                        ${libavahiclient_INCLUDE_DIRS}
                                        ${libavahicommon_INCLUDE_DIRS}
                                        )

link_directories(   )

add_library(  muroadaemon SHARED  ${SRC} ${HEADERS} )
target_link_libraries( muroadaemon  ${Boost_LIBRARIES} 
                                    ${EXPAT_LIBRARIES}
                                    ${log4cplus_LIBRARY}
                                    pthread 
                                    ${libavahiclient_LIBRARY} 
                                    ${libavahicommon_LIBRARY}
                                    )
set_target_properties( muroadaemon  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 
# SET_TARGET_PROPERTIES( muroad PROPERTIES  LINK_FLAGS -Wl,-shared-libgcc)
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET muroadaemon
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh libmuroadaemon.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS   muroadaemon DESTINATION  lib)
