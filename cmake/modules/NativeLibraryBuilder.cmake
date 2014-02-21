#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

include(CMakeParseArguments)

macro(set_platform_folder)
  if(APPLE)
    set(native_platform "mac")
  elseif(WIN32)
    set(native_platform "win")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(native_platform "lin")
  endif()
endmacro()

set(jar_native_libraries CACHE INTERNAL "Native libraries to be built into the JAR")

# Adds a native library into the jar file.
macro(jar_native_library lib)
  get_property(_soversion TARGET ${lib} PROPERTY SOVERSION)
  get_property(_version TARGET ${lib} PROPERTY VERSION)
  get_property(_location TARGET ${lib} PROPERTY LOCATION)
  get_property(_ncfg TARGET ${lib} PROPERTY IMPORTED_SONAME_NOCONFIG)
  set_platform_folder()

  # if the lib is imported, we know what it is called!
  if(_ncfg)
      set(_output_name ${_ncfg})
  else()
    get_filename_component(_basename ${_location} NAME_WE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      set(_output_name ${_basename}.so.${_soversion})
    elseif(APPLE)
      set(_output_name ${_basename}.${_soversion}.dylib)
    elseif(WIN32)
      set(_output_name ${_basename}.dll)
    endif()
  endif()

  # add rpath to linux flags
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--rpath -Wl,\$ORIGIN")
  endif()

  set(_src ${_location})
  set(_dest ${CMAKE_BINARY_DIR}/lib/${native_platform}/${_output_name})
  add_custom_command(OUTPUT  ${_dest}
                     COMMAND cmake -E copy_if_different
                     ARGS    ${_src} ${_dest}
                     DEPENDS ${lib}
                     COMMENT "Installing ${_output_name} in JAR lib directory")
  set(_target ${_output_name}_jar_native_library)
  add_custom_target(${_target} ALL DEPENDS ${_dest})
  list(APPEND jar_native_libraries ${_target})
  set(jar_native_libraries ${jar_native_libraries} CACHE INTERNAL "Native libraries to be built into the JAR")
endmacro()

# Adds the lib for each target into the final built jar file.
macro(jar_native_multitarget_library lib)
  cmake_parse_arguments(JNMTLIB "" "" "TARGETS" ${ARGN})
  foreach(TARGET ${JNMTLIB_TARGETS})
    jar_native_library(${lib}_${TARGET})
  endforeach()
endmacro()
