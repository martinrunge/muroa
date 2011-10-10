cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )

add_definitions( -D__STDC_CONSTANT_MACROS )

set( SRCS   main.cpp  )
                    
                    
set( HEADERS         )                    
  

#include ( ${QT_USE_FILE} )
#qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS}) 

link_directories(${BINARY_DIR} "${CMAKE_SOURCE_DIR}/../../build" )

ADD_EXECUTABLE(rehash  ${SRCS} ${HEADERS} )

set_target_properties( rehash PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(rehash       muroa++ 
                                   xdiff
                                   ${Boost_LIBRARIES}  )
