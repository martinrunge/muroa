cmake_minimum_required (VERSION 2.6)
project (asiotest)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS system regex REQUIRED )
find_package( EXPAT REQUIRED )

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
                   
include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${EXPAT_INCLUDE_DIRS})

link_directories(  )

add_library(  muroad SHARED  ${SRC} ${HEADERS} )
target_link_libraries( muroad  ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} log4cplus pthread avahi-client avahi-common)
set_target_properties( muroad  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 

install(TARGETS   muroad DESTINATION  lib)
