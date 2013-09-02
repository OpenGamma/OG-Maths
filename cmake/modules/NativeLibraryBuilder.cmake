#
# Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

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

macro(jar_native_library lib)
  get_property(_soversion TARGET ${lib} PROPERTY SOVERSION)
  get_property(_location TARGET ${lib} PROPERTY LOCATION)
  set_platform_folder()

  get_filename_component(_basename ${_location} NAME_WE)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(_output_name ${_basename}.so.${_soversion})
  elseif(APPLE)
    set(_output_name ${_basename}.${_soversion}.dylib)
  elseif(WIN32)
    set(_output_name ${_basename}.dll)
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
  message("Native libs now ${jar_native_libraries}")
endmacro()
