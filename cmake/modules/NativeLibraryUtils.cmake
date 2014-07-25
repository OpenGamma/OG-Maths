#
# Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
#
# Please see distribution for license.
#

macro(set_platform_code PC_VAR)
  if(APPLE)
    set(${PC_VAR} "mac")
  elseif(WIN32)
    if(BITS32)
      set(${PC_VAR} "w32")
    else()
      set(${PC_VAR} "win")
    endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(${PC_VAR} "lin")
  endif()
endmacro()
