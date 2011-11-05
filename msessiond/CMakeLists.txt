cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system REQUIRED )

add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             CSession.cpp
                      CConnection.cpp
                      CRpc.cpp
                      main.cpp
                      )
                    
                   
set( HEADERS          CSession.h
                      CConnection.h
                      CRpc.h)                    
  

include_directories(${SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../libmuroad ${CMAKE_SOURCE_DIR}/../libmuroa++) 

link_directories(${BINARY_DIR} "${CMAKE_SOURCE_DIR}/../libmuroad/build" "${CMAKE_SOURCE_DIR}/../libmuroa++/build")

ADD_EXECUTABLE(msessiond ${SRCS} ${HEADERS} )

set_target_properties( msessiond PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiond    muroad
                                   muroa++
                                   ${Boost_LIBRARIES} 
                                   xdiff
                                   log4cplus
                                   )
