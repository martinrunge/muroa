cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

set(VERBOSE 1)

find_package(PkgConfig)


find_package( libavcodec REQUIRED )
find_package( libavformat REQUIRED )
find_package( libavutil REQUIRED )
find_package( ZLIB REQUIRED )
find_package( BZip2 REQUIRED )
find_package( GCRYPT REQUIRED )

find_package( LibAO REQUIRED)
find_package( LibSndFile REQUIRED)
find_package( LibSpeex REQUIRED)

add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS           main.cpp 
                    CSpeexResampler.cpp
                    CMuroaFPResampler.cpp
                    muroafp/cfixpointresampler.cpp
                    muroafp/cfilterdef.cpp
                    CSoxResampler.cpp
                    IResamplerBase.cpp
                    ) 
                  

set( HEADERS        IResamplerBase.h
                    CMuroaFPResampler.h
                    muroafp/cfixpointresampler.h
                    muroafp/CFixPointResamplerLargeFilter.h
                    muroafp/CFixPointResamplerSmallFilter.h
                    muroafp/cfilterdef.h
                    CSpeexResampler.h
                    CSoxResampler.h
                    )                    


include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/speex 
                    ${libavcodec_INCLUDE_DIR}
                    ${libavformat_INCLUDE_DIR}
                    ${libavutil_INCLUDE_DIR}
                    ${LibAO_INCLUDE_DIRS}
                    ${LIBSNDFILE_INCLUDE_DIR}
                    ${LibSpeex_INCLUDE_DIR}
                    )
                
link_directories(${BINARY_DIR} 
                 )

ADD_EXECUTABLE(resamplertest ${SRCS} ${HEADERS} )

set_target_properties( resamplertest PROPERTIES  COMPILE_FLAGS  "-std=gnu++0x") 
set_property( SOURCE speex/resample.c APPEND PROPERTY  COMPILE_FLAGS "-DFLOATING_POINT -DEXPORT" ) 


TARGET_LINK_LIBRARIES(resamplertest    ${libavformat_LIBRARY}
                                       ${libavcodec_LIBRARY}
                                       ${libavutil_LIBRARY}
                                       ${LIBSNDFILE_LIBRARY}
                                       ${LibSpeex_LIBRARY}
                                       ${ZLIB_LIBRARY}
                                       ${BZIP2_LIBRARIES}
                                       ${GCRYPT_LIBRARIES}
                                       )                                   
                                   
