# - Find library containing TagLib()
# The following variables are set if Taglib is found. If TagLib is not
# found, TagLib_FOUND is set to false.
#  TagLib_FOUND     - System has Taglib.
#  TagLib_LIBRARIES - Link these to use Taglib.
#  TagLib_INCLUDE_DIRS - include directories
#  TagLib_DEFINITIONS - Link these to use Taglib.


if (NOT TagLib_SEARCHED)
    include(CheckLibraryExists)

    set(TagLib_SEARCHED TRUE CACHE INTERNAL "")
    set(TagLib_FOUND FALSE CACHE INTERNAL "")

    message(STATUS "PKG_CHECK  Taglib")

    pkg_check_modules(TagLib taglib)

    message(STATUS "past PKG_CHECK  Taglib")

    set(TagLib_DEFINITIONS ${PC_TagLib_CFLAGS_OTHER})

    find_path(TagLib_INCLUDE_DIR taglib/taglib.h
          HINTS ${PC_TagLib_INCLUDEDIR} ${PC_TagLib_INCLUDE_DIRS}
          PATH_SUFFIXES taglib )

    find_library(TagLib_LIBRARY NAMES tag
             HINTS ${PC_TagLib_LIBDIR} ${PC_TagLib_LIBRARY_DIRS} )

    set(TagLib_LIBRARIES ${TagLib_LIBRARY} )
    set(TagLib_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )

    include(FindPackageHandleStandardArgs)
    # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(TagLib  DEFAULT_MSG
                                      TagLib_LIBRARIES      TagLib_INCLUDE_DIRS)

    
    
    if (TAGLIB_FOUND)
      if (NOT TAGLIB_FIND_QUIETLY)
        message(STATUS "Found Taglib in: ${Taglib_LIBRARIES}")
      endif (NOT TAGLIB_FIND_QUIETLY)
    else (TAGLIB_FOUND)
      if (TAGLIB_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find the library containing Taglib")
      endif (TAGLIB_FIND_REQUIRED)
    endif (TAGLIB_FOUND)

    mark_as_advanced(Taglib_LIBRARIES)
    mark_as_advanced(Taglib_INCLUDE_DIRS)
endif(NOT TagLib_SEARCHED)
