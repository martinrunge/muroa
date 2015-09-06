cmake_minimum_required (VERSION 2.6)

project (muroadaemon)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )

set(Boost_USE_MULTITHREADED ON)
find_package( Threads )
find_package( Boost 1.36.0 COMPONENTS system filesystem  REQUIRED )
find_package( EXPAT REQUIRED )
find_package( libavahiclient REQUIRED )
find_package( libavahicommon REQUIRED )
find_package( log4cplus REQUIRED )

MESSAGE(STATUS "log4cplus_DEFINITIONS: ${log4cplus_DEFINITIONS}")

add_definitions( ${log4cplus_DEFINITIONS} )


set( SRC           CTcpConnection.cpp
                   CTcpServer.cpp
                   CConnectionManager.cpp
                   CSignalHandler.cpp
                   CSettings.cpp
                   Exceptions.cpp
                   CApp.cpp
                   CAppenderErrorHandler.cpp
                   # main.cpp
                   # avahi/asio-watch.cpp
                   avahi/CDnsSdBase.cpp
                   avahi/CDnsSdAvahi.cpp
                   avahi/CServiceDesc.cpp
                   )
                  

set( HEADERS       CTcpConnection.h
                   CTcpServer.h
                   IConnectionManager.h
                   CConnectionManager.cpp
                   CSignalHandler.h
                   CSettings.h
                   Exceptions.h
                   CApp.h
                   CAppenderErrorHandler.h
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
target_link_libraries( muroadaemon  ${CMAKE_THREAD_LIBS_INIT} 
                                    ${Boost_LIBRARIES} 
                                    ${EXPAT_LIBRARIES}
                                    ${log4cplus_LIBRARY}
                                    ${libavahiclient_LIBRARY} 
                                    ${libavahicommon_LIBRARY}
                                    )

set_target_properties( muroadaemon  PROPERTIES  COMPILE_FLAGS "-std=c++0x -pthread" ) 

ADD_GDB_INDEX( muroadaemon )

install(TARGETS   muroadaemon DESTINATION  lib)
