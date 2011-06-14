cmake_minimum_required (VERSION 2.6)
project (treemodel)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


set( SRC           CTreeModel.cpp
                   CMainWindow.cpp
                   main.cpp )
                  

set( MOC_H         CTreeModel.h
                   CMainWindow.h
                   )
                   
SET(UIS            MainWindow.ui  
                   )

set(RCCS           # MainWindow.qrc 
                   )      

QT4_WRAP_UI(UIS_H ${UIS})

QT4_ADD_RESOURCES( RCCS_SRC  ${RCCS} )

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../libmuroa++)

link_directories( ${QT_LIB_DIR} ${CMAKE_SOURCE_DIR}/../libmuroa++/build)

add_executable( treemodel ${SRC} ${MOC_SOURCES} ${UIS_H} ${RCCS_SRC})
target_link_libraries( treemodel   muroa++
                                   xdiff
                                   ${QT_LIBRARIES} )


set_target_properties( treemodel  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   treemodel
        DESTINATION  bin)
