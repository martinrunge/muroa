cmake_minimum_required (VERSION 2.6)
project (asiotest)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS system regex REQUIRED )
find_package( EXPAT REQUIRED )

add_definitions(  )


set( SRC           CTcpConnection.cpp
                   CTcpServer.cpp
                   CConnectionManager.cpp
                   CSignalHandler.cpp
                   CSettings.cpp
                   Exceptions.cpp
                   main.cpp
                   # avahi/asio-watch.cpp 
                   avahi/CAvahiThread.cpp
                   )
                  

set( HEADERS       CTcpConnection.h
                   CTcpServer.h
                   CConnectionManager.h
                   CSignalHandler.h
                   CSettings.h
                   Exceptions.h
                   # avahi/asio-watch.h
                   avahi/CAvahiThread.h
                   )
                   
include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${EXPAT_INCLUDE_DIRS})

link_directories(  )

add_executable( asiotest ${SRC} ${HEADERS} )
target_link_libraries( asiotest  ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} -llog4cplus)
set_target_properties( asiotest  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   asiotest DESTINATION  bin)
