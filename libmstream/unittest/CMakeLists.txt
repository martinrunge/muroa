cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui )
find_package ( CppUnit REQUIRED )

add_definitions( ${QT4_DEFINITIONS} -D__STDC_CONSTANT_MACROS ${CPPUNIT_DEFINITIONS})

set( unittests_SRCS   CCmdStreamTests.cpp
                      CStreamSMTest.cpp
                      CStreamCtrlRpcDummy.cpp
                      LibMStreamTestApp.cpp
                      )
                    
set( unittests_H   
                      )
                    
set( HEADERS          CCmdStreamTests.h
                      CStreamSMTest.h
                      CStreamCtrlRpcDummy.h 
                      )                    
  

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmcommons
                    ${Boost_INCLUDE_DIRS} 
                    ${CMAKE_CURRENT_BINARY_DIR}/..
                    ${CPPUNIT_INCLUDE_DIRS} ) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../build" )

ADD_EXECUTABLE(libmstreamtest ${unittests_SRCS} ${unittests_H} ${HEADERS} )

set_target_properties( libmstreamtest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES( libmstreamtest ${CPPUNIT_LIBRARIES} 
                                      mstream
                                      ${QT_LIBRARIES} 
                                      qxcppunitd 
                                      qxrunnerd
                                      )
