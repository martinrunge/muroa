cmake_minimum_required (VERSION 2.6)
project (libxdiff)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../aux/cmake/Modules" ${CMAKE_MODULE_PATH})


add_definitions( -DHAVE_STDIO_H  
                 -DHAVE_STDLIB_H  
                 -DHAVE_UNISTD_H
                 -DHAVE_STRING_H 
                 -DHAVE_MEMCHR
                 -DHAVE_MEMCMP
                 -DHAVE_MEMCPY
                 -DHAVE_MEMSET
                 -DHAVE_STRLEN 
                 -DPACKAGE_VERSION="'0.23'")


set( SRC        xdiff/xalloc.c
                xdiff/xmerge3.c
                xdiff/xrabdiff.c
                xdiff/xrabply.c
                xdiff/xbdiff.c
                xdiff/xbpatchi.c
                xdiff/xdiffi.c
                xdiff/xemit.c
                xdiff/xmissing.c
                xdiff/xpatchi.c
                xdiff/xprepare.c
                xdiff/xutils.c
                xdiff/xversion.c
                xdiff/xadler32.c  )
                  

set( HEADERS    xdiff/xbdiff.h
                xdiff/xdiff.h
                xdiff/xdiffi.h
                xdiff/xemit.h
                xdiff/xinclude.h
                xdiff/xmacros.h
                xdiff/xmissing.h
                xdiff/xprepare.h
                xdiff/xtypes.h
                xdiff/xutils.h
                xdiff/xadler32.h  )
                   
include_directories(  )

link_directories(  )

add_library( xdiff SHARED ${SRC} ${HEADERS} )
# target_link_libraries( muroa++  ${Boost_LIBRARIES} ${EXPAT_LIBRARIES})
# set_target_properties( muroa++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" )

###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET xdiff
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../aux/scripts/mkgdbindex.sh libxdiff.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   
 

install(TARGETS   xdiff DESTINATION  lib)
