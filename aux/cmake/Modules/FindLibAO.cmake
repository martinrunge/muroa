
MESSAGE(STATUS "Looking for LibAO")

FIND_PATH(LibAO_INCLUDE_DIR ao.h /usr/include/ao /usr/local/include/ao)
FIND_LIBRARY(LibAO_LIBRARY NAMES ao PATH /usr/lib /usr/local/lib) 


# handle the QUIETLY and REQUIRED arguments and set LIBAO_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibAO  DEFAULT_MSG
                                  LibAO_LIBRARY LibAO_INCLUDE_DIR)

IF (LIBAO_FOUND)
   set(LibAO_LIBRARIES ${LibAO_LIBRARY})
   set(LibAO_INCLUDE_DIRS ${LibAO_INCLUDE_DIR})
   
   MARK_AS_ADVANCED(LibAO_LIBRARIES)
   MARK_AS_ADVANCED(LibAO_INCLUDE_DIRS)
   
ENDIF (LIBAO_FOUND)
