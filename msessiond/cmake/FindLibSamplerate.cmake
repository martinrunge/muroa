# - Find library containing LibSamplerate()
# The following variables are set if LibSamplerate is found. If LibSamplerate is not
# found, LibSamplerate_FOUND is set to false.
#  LibSamplerate_FOUND     - System has Taglib.
#  LibSamplerate_LIBRARIES - Link these to use Taglib.
#  LibSamplerate_CFLAGS - Link these to use Taglib.


if (NOT LibSamplerate_SEARCHED)
    include(CheckLibraryExists)

    set(LibSamplerate_SEARCHED TRUE CACHE INTERNAL "")
    set(LibSamplerate_FOUND FALSE CACHE INTERNAL "")

    pkg_check_modules(LibSamplerate samplerate )

    if (LibSamplerate_FOUND)
      if (NOT LibSamplerate_FIND_QUIETLY)
        message(STATUS "Found libsamplerate in: ${LibSamplerate_LIBRARIES}")
      endif (NOT LibSamplerate_FIND_QUIETLY)
    else (LibSamplerate_FOUND)
      if (LibSamplerate_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find the library containing LibSamplerate")
      endif (LibSamplerate_FIND_REQUIRED)
    endif (LibSamplerate_FOUND)

    mark_as_advanced(LibSamplerate_LIBRARIES)
endif(NOT LibSamplerate_SEARCHED)
