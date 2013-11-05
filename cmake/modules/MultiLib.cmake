#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include(CMakeParseArguments)

# Create a library for multiple target instruction sets. Note that this is
# specialised to its use in Nyqwk2 and does not fully provide all of the
# functionality that can be provided by add_library and set_target_properties.

function(add_multitarget_library _TARGET_NAME)
  cmake_parse_arguments(MTLIB
                        ""
                        "VERSION;SOVERSION"
                        "SOURCES;TARGETS;LINK_LIBRARIES;COMPILE_DEFINITIONS"
                        ${ARGN})
  foreach(TARGET ${MTLIB_TARGETS})
    set(LIBNAME ${_TARGET_NAME}_${TARGET})
    add_library(${LIBNAME} SHARED ${MTLIB_SOURCES})
    set_target_properties(${LIBNAME} PROPERTIES
                          VERSION ${MTLIB_VERSION}
                          SOVERSION ${MTLIB_SOVERSION}
                          COMPILE_FLAGS ${CMAKE_C_FLAGS_${TARGET}})
    foreach(LINK_LIB ${MTLIB_LINK_LIBRARIES})
      target_link_libraries(${LIBNAME} ${LINK_LIB}_${TARGET})
    endforeach()
    foreach(COMPILE_DEFINITION ${MTLIB_COMPILE_DEFINITIONS})
      set_target_properties(${LIBNAME} PROPERTIES COMPILE_DEFINITIONS ${COMPILE_DEFINITION})
    endforeach()
  endforeach()
endfunction()
