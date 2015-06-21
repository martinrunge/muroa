cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)


add_definitions( -D__STDC_CONSTANT_MACROS )

set( SRCS             main.cpp
                      server.cpp
                      )
                    
set( HEADERS          server.h )                    
  

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmcommons
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libsock++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmuroadaemon
                    ${CMAKE_CURRENT_BINARY_DIR}/..
                    ) 

link_directories(${BINARY_DIR}       "${CMAKE_SOURCE_DIR}/../build" )

ADD_EXECUTABLE(server                 ${SRCS} ${HEADERS} )

set_target_properties( server         PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES( server         mstream
                                      )
                                      
file(COPY   etc/muroa.conf  DESTINATION etc)
                                      
