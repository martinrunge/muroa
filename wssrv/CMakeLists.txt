cmake_minimum_required (VERSION 2.8)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package( Threads )
find_package( Boost 1.45.0 COMPONENTS thread random  system  REQUIRED )
find_package( JsonCpp REQUIRED )
find_package( libavcodec REQUIRED )
find_package( libavformat REQUIRED )
find_package( libavutil REQUIRED )


add_definitions( -D__STDC_CONSTANT_MACROS )

set( SRCS             main.cpp
                      CppServer.cpp
                      WSSrv.cpp
                      CHttpClient.cpp
                      CJsonResponder.cpp
                      CStreamDecoder.cpp
                      CRessourceHandler.cpp
                      CWSMsgHandler.cpp
)
                    
set( HEADERS          CppServer.h
                      WSSrv.h                    
                      CHttpClient.h
                      CJsonResponder.h
                      CStreamDecoder.h
                      CRessourceHandler.h
                      CWSMsgHandler.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmstream
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libsock++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroadaemon
                    ${CMAKE_CURRENT_BINARY_DIR}/..
                    ${Boost_INCLUDE_DIRS} 
                    ${Threads_INCLUDE_DIRS} 
                    ${JSONCPP_INCLUDE_DIRS}
                    ${libavcodec_INCLUDE_DIR}
                    ${libavformat_INCLUDE_DIR}
                    ${libavutil_INCLUDE_DIR}
) 

link_directories(${BINARY_DIR}       "${CMAKE_SOURCE_DIR}/../build" )

ADD_EXECUTABLE(wssrv                  ${SRCS} ${HEADERS} )

set_target_properties( wssrv         PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES( wssrv          mstream
                                      ${Boost_LIBRARIES} 
                                      ${Threads_LIBRARIES} 
                                      ${JSONCPP_LIBRARIES}
                                      ${libavformat_LIBRARY}
                                      ${libavcodec_LIBRARY}
                                      ${libavutil_LIBRARY}
)
                                      
file(COPY   etc/muroa.conf  DESTINATION etc)
file(COPY   log.properties  DESTINATION .)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/www
                                                           ${CMAKE_CURRENT_BINARY_DIR}/www)
                                      
ADD_GDB_INDEX( wssrv )            
                                      
install(TARGETS wssrv DESTINATION bin)
