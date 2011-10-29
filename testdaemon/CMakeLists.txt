cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )

add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS   main.cpp
                      )
                    
                   
set( HEADERS          )                    
  

include_directories(${SOURCE_DIR} .. ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/..) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../libmuroad/build" "${CMAKE_SOURCE_DIR}/../libmuroa++/build")

ADD_EXECUTABLE(muroatestd ${SRCS} ${HEADERS} )

set_target_properties( muroatestd PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(muroatestd   muroad
                                   muroa++
                                   ${Boost_LIBRARIES} 
                                   )
